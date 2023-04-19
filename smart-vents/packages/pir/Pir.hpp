/*
 * Title   : Header file for the PIR Sensor
 * File    : Pir.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

#include <iostream>
#include <stdio.h>
#include <vector>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "freertos/semphr.h"
#include "driver/gptimer.h"
#include "esp_timer.h"
#include "esp_log.h"

class Pir
{
    private:
        gpio_num_t _pin;
        uint64_t _lastTime{};
        bool _status;

        static inline std::vector<Pir*> s_pirs;

        /**
         * @brief Task that iterates through all instances of Pir (all 
         * Pir sensors on a board) and checks their status pin, 
         * changing _status to true if motion has been detected 
         * recently and false after a cooldown
        */
        static void pirTask(void* pvParameter);

    public:

        /**
         * @brief Constructor that initializes the sensor pin to input 
         * and adds the instance to a vector that is checked by a task
        */
        Pir(gpio_num_t pin);

        /**
         * @brief Initializes the task that checks the vector
        */
        void init();

        /**
         * @brief See whether this sensor has detected movement recently
         * @return true - movement has been detected recently \\
         * @return false - no movement detected
        */
        bool getStatus() const noexcept { return _status; }

        /**
         * @brief Get the assigned gpio of this sensor
         * @return 1, 2, etc. (GPIO_NUM_1, GPIO_NUM_2, etc.)
        */
        gpio_num_t getPin() const noexcept { return _pin; }

};