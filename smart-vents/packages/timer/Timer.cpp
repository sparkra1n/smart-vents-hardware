#include "Timer.hpp"

static QueueHandle_t s_queue = NULL;
static uint64_t t = 0;
static gptimer_handle_t s_timerHandle;
static std::vector<Alarm> s_alarms;

bool IRAM_ATTR Timer::alarmCb(gptimer_handle_t timer, const gptimer_alarm_event_data_t* edata, void* user_data)
{
    BaseType_t high_task_awoken = pdFALSE;
    QueueHandle_t queue = (QueueHandle_t) user_data;
    bool status = gpio_get_level(GPIO_NUM_17);
    xQueueSendFromISR(queue, &status, &high_task_awoken);

    // return whether we need to yield at the end of ISR
    return (high_task_awoken == pdTRUE);
}

void Timer::alarmTask(void* pvParameter)
{
    gpio_set_direction(GPIO_NUM_17, GPIO_MODE_INPUT);
    bool status;
    while (1)
    {
        if (xQueueReceive(s_queue, &status, 500 / portTICK_PERIOD_MS)) 
        {
            // if(status) clearPirLastActive();
            uint64_t t1 = esp_timer_get_time();
            std::cout << "status: " << status << ", time:" << t1 - t << '\n';
            t = t1;
        }
    }
}

esp_err_t Timer::init()
{
    s_queue = xQueueCreate(10, sizeof(bool));
    if (!s_queue) 
    {
        std::cout << "Creating queue failed\n";
        return ESP_FAIL;
    }

    gptimer_config_t timerConfig;
    timerConfig.clk_src = GPTIMER_CLK_SRC_DEFAULT;
    timerConfig.direction = GPTIMER_COUNT_UP;
    timerConfig.resolution_hz = 1000000; // 1MHz, 1 tick = 1us
    ESP_ERROR_CHECK(gptimer_new_timer(&timerConfig, &s_timerHandle));

    gptimer_event_callbacks_t timerEvent;
    timerEvent.on_alarm = alarmCb;
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(s_timerHandle, &timerEvent, s_queue));

    // Enable timer
    ESP_ERROR_CHECK(gptimer_enable(s_timerHandle));

    // Start timer, auto-reload at alarm event
    gptimer_alarm_config_t alarmConfig;
    alarmConfig.alarm_count = 1000000;
    alarmConfig.reload_count = 0;
    alarmConfig.flags.auto_reload_on_alarm = true;
    ESP_ERROR_CHECK(gptimer_set_alarm_action(s_timerHandle, &alarmConfig));
    
    ESP_ERROR_CHECK(gptimer_start(s_timerHandle));
    xTaskCreate(alarmTask, "timer_class", 2048, NULL, 1, NULL);
}

void Timer::addAlarm(const Alarm& alarm)
{
    // Add to list
    s_alarms.emplace_back(alarm);

    // Find GCD in order to set all interrupts as a multiple of the clock freq
    uint32_t result = s_alarms.front().period;
    for (auto it = s_alarms.begin() + 1; it != s_alarms.end(); it++)
    {
        result = std::gcd(it->period, result);
        if (result == 1)
            break;
    }
    std::cout << result;
    gptimer_alarm_config_t alarmConfig;
    alarmConfig.alarm_count = result;
    ESP_ERROR_CHECK(gptimer_set_alarm_action(s_timerHandle, &alarmConfig));
}

// esp_err_t Timer::deInit()
// {
//     std::cout << "Stop timer\n";
//     ESP_ERROR_CHECK(gptimer_stop(s_timerHandle));
//     std::cout << "Disable timer\n";
//     ESP_ERROR_CHECK(gptimer_disable(s_timerHandle));
//     std::cout << "Delete timer\n";
//     ESP_ERROR_CHECK(gptimer_del_timer(s_timerHandle));
//     vQueueDelete(s_queue);
// }
