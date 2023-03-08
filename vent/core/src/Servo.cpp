/*
 * Title   : header file for the servo control
 * File    : servo.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#include "../inc/Servo.hpp"

#include <stdio.h>
#include <cmath>
#include "driver/ledc.h"

constexpr static ledc_timer_t LEDC_TIMER = LEDC_TIMER_0;
constexpr static ledc_mode_t LEDC_SPEED_MODE = LEDC_LOW_SPEED_MODE;
constexpr static ledc_timer_bit_t LEDC_DUTY_RES = LEDC_TIMER_14_BIT;
constexpr static ledc_channel_t LEDC_CHANNEL = LEDC_CHANNEL_0;

static int getDutyCycleParam(const int& resolution, const int& percent)
{
    return (int)((std::pow(2, resolution)) - 1) * ((double) percent / 100);
}

void Servo::init()
{
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_14_BIT,
        .timer_num = LEDC_TIMER,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .gpio_num = _pin,
        .speed_mode = LEDC_SPEED_MODE,
        .channel = LEDC_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER,
        .duty = 0,
        .hpoint = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    // ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL, getDutyCycleParam(LEDC_DUTY_RES, 0));
    // ESP_ERROR_CHECK(ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL));

    // setDutyCycle(0);
}

void Servo::setDutyCycle(const uint8_t& d)
{
    ledc_set_duty(LEDC_SPEED_MODE, LEDC_CHANNEL, getDutyCycleParam(LEDC_DUTY_RES, d));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_SPEED_MODE, LEDC_CHANNEL));
}

void Servo::setAngle(const uint8_t& deg)
{
    //TODO: 10->0 25->180 or 270?

    
}

// void Servo::setFreq(const uint8_t& freq)
// {

// }