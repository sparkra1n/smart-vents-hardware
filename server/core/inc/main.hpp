/*
 * Title   : header file for the server side
 * File    : main.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

#include "../inc/comm.hpp"
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
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_now.h"
#include "esp_mac.h"
#include "esp_log.h"
#include "esp_crc.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"

typedef std::array<uint8_t, 6> MacAddress;

class VentS
{
    private:
        MacAddress _mac;
        std::string _room;
        // uint16_t _counter; // seconds since last update (cleared by server)
    public:
        VentS(MacAddress mac, std::string room) : _mac(mac), _room(room) {}
        void setMac(const MacAddress& m) { _mac = m; }
        void setRoom(const std::string& r) { _room = r; }
        // void clrCounter() { _counter = 0; }
        MacAddress getMac() const { return _mac; }
        std::string getRoom() const { return _room; }
        ~VentS() {}
};

class VentList
{
    private:
        std::vector<VentS> _list;
        void addVentImpl(const VentS& v) { _list.emplace_back(v); }
        void remVentImpl(const VentS& v) { /*TODO: impl*/ }
        std::vector<VentS>& getVentsImpl() {return _list; } 
        VentList() {}
        ~VentList() {}
    public:
        VentList(const VentList&) = delete;
        VentList(VentList&&) = delete;
        VentList& operator=(const VentList&) = delete;
        VentList& operator=(VentList&&) = delete;
        static VentList& get() 
        {
            static VentList ventList; 
            return ventList;
        }
        static inline void addVent(const VentS& v) { get().addVentImpl(v); }
        static inline void remVent(const VentS& v) { get().remVentImpl(v); }
        static inline std::vector<VentS>& getVents() { return get().getVentsImpl(); }

};

class Server final
{
    private:
        Server() {}
        ~Server() {}
    public:
        static Server& get() 
        { 
            static Server server; 
            return server;
        }
};
