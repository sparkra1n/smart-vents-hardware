/*
 * Title   : Header File for Server
 * File    : Server.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

#include "../../common/EspNow.hpp"

#include <algorithm>
#include <array>
#include <string>
#include <vector>
#include <stdio.h>

class Server final
{
    private:
        Server() {}
    public:
        Server(const Server&) = delete;
        Server(Server&&) = delete;
        Server& operator=(const Server&) = delete;
        Server& operator=(Server&&) = delete;
        static void espNowRecvCb(const Message& message);
        static void init();
        static Server& get() { static Server server; return server; }
};