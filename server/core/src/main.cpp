/*
 * Title   : source file for the server side
 * File    : main.cpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#include "../inc/main.hpp"
#include "../inc/comm.hpp"

#define IDLE_TASK_SIZE 1000

namespace server
{
    void init()
    {
        gpio_set_direction(GPIO_NUM_21, GPIO_MODE_OUTPUT);
        gpio_set_direction(GPIO_NUM_19, GPIO_MODE_INPUT);
    }
}

// void print_mac(const MacAddress& m) 
// {
//     printf("MAC Address: %20X:%20X:%20X:%20X:%20X:%20X\n", m[0], m[1], m[2], m[3], m[4], m[5]);
// }

bool isPressed(const gpio_num_t& gpio)
{
    for (int i = 0; i < 200; i++)
    {
        if (gpio_get_level(gpio)) // Catch at pressed state
        {
            vTaskDelay(50 / portTICK_PERIOD_MS); // Wait a while
            if (gpio_get_level(gpio)) // Check again
            {
                while(gpio_get_level(gpio)); // Wait for user to release button
                return true;
            }
        }
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    return false;
}

extern "C" void app_main()
{
    // MacAddress serverMacAddress;
    // unsigned char mac_base[6] = {0};
    // esp_efuse_mac_get_default(mac_base);
    // esp_read_mac(mac_base, ESP_MAC_WIFI_STA);
    // serverMacAddress = std::to_array(mac_base);
    // print_mac(serverMacAddress);

    ESP_LOGI("init", "initializing server...");
    server::init();
    ESP_LOGI("init", "initialized server");

    ESP_LOGI("init", "initializing comm...");
    EspNow::init();
    ESP_LOGI("init", "initialized comm...");

    VentS vent({0x70, 0x04, 0x1d, 0xfb, 0xde, 0x48}, "closet");
    VentList::addVent(vent);

    while (1)
    {
        // gpio_set_level(GPIO_NUM_21, 1);
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
        // gpio_set_level(GPIO_NUM_21, 0);
        // vTaskDelay(1000 / portTICK_PERIOD_MS);

        if (isPressed(GPIO_NUM_19))
        {
            ESP_LOGI("button", "pressed");
            // int i = EspNow::test();
            // printf("test: %d", i);
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
