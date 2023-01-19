/*
 * File     : server.h
 * Authors  : Ani
 * Date     : 1/6/23
*/

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#include <algorithm>
#include <string>
#include <vector>
#include <array>
#include <stdio.h>

#pragma once

typedef __uint8_t[6] macAddress;

typedef enum Action 
{
    OPEN,  // Open vent
    CLOSE, // Close vent
    GET,   // Get temp
    SET,   // Set temp
} Action;

typedef struct ServerPayload
{

} ServerPayload;

typedef struct ClientPayload
{
    Action action;
} ClientPayload;

class VentS 
{
    
}

class Server
{
    Server() : vents(ventList), ssid(ssid), pswd(pswd) {}
    inline void addVent(const VentS& v);
    inline void removeVent(const VentS& v);
};
