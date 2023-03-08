/*
 * Title   : Header File for the Vents on the Server Side
 * File    : SxVent.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

#include "../../common/EspNow.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdio.h>
#include <assert.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "esp_now.h"
#include "esp_mac.h"
#include "esp_log.h"
#include "esp_crc.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"

class SVent
{
    private:
        MacAddress _mac;
        std::string _room;
        // uint16_t _counter; // seconds since last update (cleared by server)
    public:
        SVent(MacAddress mac, std::string room) : _mac(mac), _room(room) {}
        void setMac(const MacAddress& m) { _mac = m; }
        void setRoom(const std::string& r) { _room = r; }
        // void clrCounter() { _counter = 0; }
        MacAddress getMac() const { return _mac; }
        std::string getRoom() const { return _room; }
        ~SVent() {}
};

class VentList
{
    private:
        std::vector<SVent> _list;
        void addVentImpl(const SVent& v);
        void remVentImpl(const SVent& v);
        std::vector<SVent>& getVentsImpl() {return _list; } 
        VentList() {}
    public:
        VentList(const VentList&) = delete;
        VentList(VentList&&) = delete;
        VentList& operator=(const VentList&) = delete;
        VentList& operator=(VentList&&) = delete;
        static VentList& get() { static VentList ventList; return ventList; }
        static void addVent(const SVent& v) { get().addVentImpl(v); }
        static void remVent(const SVent& v) { get().remVentImpl(v); }
        static std::vector<SVent>& getVents() { return get().getVentsImpl(); }
};
