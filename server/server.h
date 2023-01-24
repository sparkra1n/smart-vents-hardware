/*
 * File     : server.h
 * Authors  : Ani
 * Date     : 1/6/23
*/

#pragma once

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
}

class Server
{
    Server() : vents(ventList), ssid(ssid), pswd(pswd) {}
    void addVent(const VentS& v);
    void removeVent(const VentS& v);
};
