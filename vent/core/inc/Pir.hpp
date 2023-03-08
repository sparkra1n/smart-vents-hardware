/*
 * Title   : Header file for the PIR Sensor
 * File    : Pir.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

constexpr short ESP_INTR_FLAG_DEFAULT = 0;
typedef void(*pirIntr)();

class Pir
{
    private:
        const gpio_num_t _pin;
    public:
        void addIntr(pirIntr);
        void init();
        constexpr Pir(gpio_num_t pin) : _pin(pin) 
        {
            //TODO: Set pin to input
        }
};