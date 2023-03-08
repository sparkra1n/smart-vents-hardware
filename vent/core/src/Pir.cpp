#include "../inc/Pir.hpp"

static QueueHandle_t s_gpio_evt_queue = NULL;

// static void IRAM_ATTR gpio_isr_handler(void* arg)
// {
//     uint32_t gpio_num = (uint32_t) arg;
//     xQueueSendFromISR(s_gpio_evt_queue, &gpio_num, NULL);
// }

// static void gpio_task_example(void* arg)
// {
//     uint32_t io_num;
//     while (1)
//     {
//         if (xQueueReceive(s_gpio_evt_queue, &io_num, 500)) 
//         {
//             //TODO: clear timer
//             printf("a\n");

//         }
//     }
// }


void Pir::addIntr(pirIntr)
{
    
}

void Pir::init()
{
    ESP_LOGI("Pir", "initing");
    
    // gpio_config_t io_conf = {
    //     .pin_bit_mask = 1ULL << _pin,
    //     .mode = GPIO_MODE_INPUT,
    //     .pull_up_en = GPIO_PULLUP_DISABLE,
    //     .intr_type = GPIO_INTR_POSEDGE
    // };

    // gpio_config(&io_conf);
    
    // s_gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    // xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);
    // gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    // gpio_isr_handler_add(_pin, gpio_isr_handler, (void*) _pin);
}