/*
 * File     : server.cpp
 * Authors  : Ani
 * Date     : 1/6/23
*/

#include "server.h"
#include "../payload.h"

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#include <algorithm>
#include <string>
#include <vector>
#include <array>
#include <stdio.h>

class VentS
{
    private:
        std::array<__uint8_t, 6> mac;
        std::string room;
    public:
        VentS() : mac(mac), room(room) {}

        void sendCallback(const uint8_t *mac_addr, esp_now_send_status_t status) 
        {
            if (status != ESP_NOW_SEND_SUCCESS)
                throw networkIOException("Unable to send payload to " + mac);
        }

        inline void registerCallback()
        {
            esp_now_register_send_cb(sendCallback);
        }

        void send(const Payload& p) const
        {
            
        }

        const Payload& get()
        {

        }
};

class Server
{
    private:
        std::array<__uint8_t, 6> mac;
        std::vector<VentS> ventList;
    public:
        Server() : vents(ventList), ssid(ssid), pswd(pswd) {}

        void setMac()
        {

        }

        void addVent(const VentS& v)
        {
            esp_now_register_send_cb(OnDataSent);
            memcpy(peerInfo.peer_addr, broadcastAddress, 6);
            peerInfo.channel = 0;
            peerInfo.encrypt = false;
        
            if (esp_now_add_peer(&peerInfo) != ESP_OK){
                throw networkIOException("Unable to connect to peer");
            }
            ventList.emplace_back(v);
        }

        void removeVent(const VentS& v)
        {
            ventList.erase(std::remove(ventList.begin(), ventList.end(), v), ventList.end());
        }
};

namespace idle // Server interrupt functions
{
    hw_timer_t *timerInterrupt = NULL;

    void IRAM_ATTR onTimer()
    {
        // Update display
    }

    void init()
    {
        timerInterrupt = timerBegin(0, 80, true);
        timerAttachInterrupt(timerInterrupt, &onTimer, true);
        timerAlarmWrite(timerInterrupt, 1000000, true);
        timerAlarmEnable(timerInterrupt);
    }


}
