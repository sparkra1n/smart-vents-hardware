/*
 * Title   : Main file for the Vent Side
 * File    : Main.cpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#include "../inc/Vent.hpp"
#include "esp_timer.h"

#define IDLE_TASK_SIZE 1000

extern "C" void app_main()
{
    Vent::init();
    while (1) 
    {
        // uint64_t t = esp_timer_get_time();
        // ESP_LOGI("Main", "t: %lld", t);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}