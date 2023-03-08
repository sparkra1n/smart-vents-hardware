/*
 * Title   : header file for the vent side
 * File    : Vent.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

#include "../inc/Temp.hpp"
#include "../inc/Servo.hpp"
#include "../inc/Pir.hpp"
#include "../../common/EspNow.hpp"
#include <algorithm>
#include <array>
#include <vector>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "sdkconfig.h"

class Vent final
{
    private:
        static void glbIntrCb();
        static void pirIntrCb();
        static void openImpl();
        static void closeImpl();
        Vent() {}
    public:
        Vent(const Vent&) = delete;
        Vent(Vent&&) = delete;
        Vent& operator=(const Vent&) = delete;
        Vent& operator=(Vent&&) = delete;
        static void init();
        static Vent& get() { static Vent vent; return vent; }
        static void open() { get().openImpl(); }
        static void close() { get().closeImpl(); }
};