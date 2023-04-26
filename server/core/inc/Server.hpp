/*
 * Title   : Header File for Server
 * File    : Server.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

#include "EspNow.hpp"
#include "SVent.hpp"

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

        static inline std::vector<SVent> s_ventList;
        
        static inline float setTemp = 25.0f;
};