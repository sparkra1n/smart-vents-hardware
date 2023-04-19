/*
 * Title   : Source File for Server
 * File    : Server.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#include "../inc/Server.hpp"
#include "../inc/SVent.hpp"

static std::vector<SVent> s_ventList;
static float setTemp = 20.0f;

void Server::packetReader(void* pvParameter)
{
    while(1)
    {
        MacAddress mac;
        Message msg;

        if (EspNow::read(mac, msg))
        {
            std::cout << "Received message from " << mac 
                    << ", temp: "                 << msg.tempc 
                    << ", motion detected: "      << msg.hasMotion
                    << ", is open: "              << msg.isOpen
                    << '\n';

            for (auto& vent : s_ventList)
            {
                if (vent.getMac() == mac)
                {
                    vent.setVals(msg.tempc, msg.hasMotion, msg.isOpen);
                    break;
                }
            }
        }

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void Server::initI()
{
    ESP_ERROR_CHECK(EspNow::init());
    xTaskCreate(packetReader, "packet_reader", 2048, NULL, 4, NULL);
    // ESP_ERROR_CHECK(Uart::init(0, GPIO_NUM_10, GPIO_NUM_11, 115200, 1024));

    s_ventList.emplace_back( SVent("room 1", {0x70, 0x04, 0x1d, 0xfb, 0xde, 0x48}) );
    s_ventList.emplace_back( SVent("room 2", {0x70, 0x04, 0x1d, 0xfb, 0xdd, 0xe4}) );

    for (auto& vent : s_ventList)
        ESP_ERROR_CHECK(EspNow::addPeer(vent.getMac()));
}

void Server::loopI()
{
    #if 1
    for (auto& vent : s_ventList)
    {

        // Close any open vents that have no motion detected and move on
        if (!vent.getMotionState())
        {
            if (vent.getOpenState())
            {
                std::cout << "a\n";
                ESP_ERROR_CHECK(vent.close());
            }
        }

        // Motion has been detected
        else
        {
            if (vent.getTemp() < setTemp && !vent.getOpenState())
            {
                std::cout << "b\n";
                ESP_ERROR_CHECK(vent.open());
            }

            else if (vent.getTemp() >= setTemp && vent.getOpenState())
            {
                std::cout << "c\n";
                ESP_ERROR_CHECK(vent.close());
            }
        }
    }

    #endif

    vTaskDelay(1000 / portTICK_PERIOD_MS);
}
