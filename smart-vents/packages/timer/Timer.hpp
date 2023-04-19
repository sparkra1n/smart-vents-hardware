#include <stdio.h>
#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <numeric>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gptimer.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"

#pragma once

struct Alarm
{
    uint32_t period;
    std::function<void()> cb;
    std::string name;
};

class Timer
{
    private:
        Timer() {}
        static bool IRAM_ATTR alarmCb(gptimer_handle_t timer, const gptimer_alarm_event_data_t* edata, void* user_data);
        static void alarmTask(void* pvParameter);
    public:
        Timer(const Timer&) = delete;
        Timer(Timer&&) = delete;
        Timer& operator=(const Timer&) = delete;
        Timer& operator=(Timer&&) = delete;

        esp_err_t init();
        void addAlarm(const Alarm& alarm);
        // esp_err_t deInit();
};