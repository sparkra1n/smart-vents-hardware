/*
 * Title   : Source File for the Server Side
 * File    : Main.cpp
 * Author  : Ani
 * Date    : 01/30/23
*/

//TODO: com.cpp prepare data: make it return espfail if len too short and use nodiscard
//TODO: make espnow::send take in more sendparams

#include "../inc/Server.hpp"
#include "../inc/SxVent.hpp"
#include "../../common/EspNow.hpp"

#define IDLE_TASK_SIZE 1000

extern "C" void app_main()
{
    Server::init();
    ESP_ERROR_CHECK(EspNow::init());
    EspNow::attachRecvCb(Server::espNowRecvCb);

    // VentS vent({0x70, 0x04, 0x1d, 0xfb, 0xde, 0x48}, "closet");
    // VentList::addVent(vent);

    #define SERVER 0
    #if SERVER
        static std::vector<MacAddress> macs = {
            { 0x70, 0x04, 0x1d, 0xfb, 0xde, 0x48 }, 
            { 0x70, 0x04, 0x1d, 0xfb, 0xdd, 0xe4 }
        };
    #else
        static std::vector<MacAddress> macs = {
            { 0x70, 0x04, 0x1d, 0xfb, 0xde, 0x30 }, 
            { 0x70, 0x04, 0x1d, 0xfb, 0xdd, 0xe4 }
        };
    #endif

    for (const auto& mac : macs)
        ESP_ERROR_CHECK(EspNow::addPeer(mac));

    Message message = { 
        .task = 0x01,
        .action = 0x01
    };

    while (1)
    {
        for (const auto& mac : macs)
            ESP_ERROR_CHECK(EspNow::send(mac, message));
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}
