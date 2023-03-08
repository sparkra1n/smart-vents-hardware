#pragma once

#include <stdio.h>
#include "driver/ledc.h"

class Servo
{
    private:
        const gpio_num_t _pin; // Signal output pin
    public:
        void init();
        void setDutyCycle(const uint8_t& duty);
        void setAngle(const uint8_t& deg);
        // void setFreq(const uint8_t& freq);
        constexpr Servo(gpio_num_t pin) : _pin(pin) {}

};