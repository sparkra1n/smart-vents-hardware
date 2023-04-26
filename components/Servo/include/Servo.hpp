/*
 * Title   : Header File for Servo Control
 * File    : Servo.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

#include <stdio.h>
#include "driver/ledc.h"
#include "esp_log.h"

constexpr static const char* TAG = "Servo.hpp";

class Servo
{
    private:
        const gpio_num_t _pin; // Signal output pin
        void setDutyCycle(const double& duty);        
    public:

        /**
         * @brief initialize PWM timer control for signal generation
        */
        void init();

        /**
         * @brief set servo position
         * @param degree angle to be set
        */
        void setAngle(uint16_t degree);
        constexpr Servo(gpio_num_t pin) : _pin(pin) {}

};