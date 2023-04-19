/*
 * Title   : Source File for the Server Side
 * File    : Main.cpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#include "../inc/Server.hpp"
#include "esp_log.h"

#define IDLE_TASK_SIZE 1000

extern "C" void app_main()
{
    ESP_LOGI("Main.cpp", "Starting...");
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    Server::init();

    while (1)
    {
        Server::loop();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
