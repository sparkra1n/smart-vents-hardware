/*
 * Title   : Vent Source File for the Vent Side
 * File    : Vent.cpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#include <stdio.h>
#include "../inc/Vent.hpp"

static constexpr gpio_num_t PIR_PIN = GPIO_NUM_4;
static constexpr gpio_num_t SERVO_PIN = GPIO_NUM_5;
static Servo s_servo = Servo(SERVO_PIN);
static Pir s_pir = Pir(PIR_PIN);

void Vent::glbIntrCb()
{

}

void Vent::pirIntrCb()
{

}

void Vent::openImpl()
{

}

void Vent::closeImpl()
{

}

void Vent::init()
{
    s_servo.init();
    s_servo.setDutyCycle(10);
    // s_pir.init();
}