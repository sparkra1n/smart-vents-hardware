/*
 * File     : server.cpp
 * Authors  : Ani
 * Date     : 1/6/23
*/

#include "server.h"

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#include <stdio.h>
#include <vector>
#include <string>
#include <array>

class VentS
{
    private:
        std::array<__uint8_t, 6> mac;
        std::string room;
    public:
        VentS() : mac(mac), room(room) {}
        void send() 
        {

        }
};

class Server
{
    private: 
        std::vector<VentS> ventList;
    public:
        Server() : vents(ventList), ssid(ssid), pswd(pswd) {}
        inline void addVent(const VentS& v)
        {
            ventList.emplace_back(v);
        }

        inline void removeVent(const VentS& v)
        {
            ventList.erase(std::remove(ventList.begin(), ventList.end(), v), ventList.end());
        }
        
};

int main()
{
    while (1)
    {

    }
}
