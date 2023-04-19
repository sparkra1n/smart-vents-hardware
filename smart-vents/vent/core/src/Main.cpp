/*
 * Title   : Main file for the Vent Side
 * File    : Main.cpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#include "../inc/Vent.hpp"
#include "esp_timer.h"
#include "esp_log.h"

#define IDLE_TASK_SIZE 1000

extern "C" void app_main()
{
    ESP_LOGI("Main.cpp", "initing");
    Vent::init();

    while (1) 
    {
        Vent::loop();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}