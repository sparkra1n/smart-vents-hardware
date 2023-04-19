/*
 * Title   : Header File for Server
 * File    : Server.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

#include "../../packages/espnow/EspNow.hpp"
#include "../../packages/uart/Uart.hpp"
#include "../../packages/timer/Timer.hpp"

#include <iostream>
#include <vector>
#include <stdio.h>

class Server
{
    private:
        Server() {}

        /**
         * @brief Task to read the buffer of incoming EspNow data 
        */
        static void packetReader(void* pvParameter);

        static void initI();

        static void loopI();

    public:
        Server(const Server&) = delete;
        Server(Server&&) = delete;
        Server& operator=(const Server&) = delete;
        Server& operator=(Server&&) = delete;

        /**
         * @brief Returns single instance of Server
        */
        static Server& get() { static Server server; return server; }

        /**
         * @brief Initializing code called from the entry point app_main() that runs once
        */
        static void init() { get().initI(); }

        /**
         * @brief Loop that is repeatedly called from the entry point app_main()
        */
        static void loop() { get().loopI(); }
};