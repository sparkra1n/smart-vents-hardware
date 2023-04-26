/*
 * Title   : header file for the vent side
 * File    : Vent.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

#include "EspNow.hpp"
#include "Servo.hpp"
#include "Pir.hpp"
#include "Bme.hpp"
#include "BmeI2c.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "sdkconfig.h"

class Vent
{
    private:
        inline static bool isOpen = false;

        /**
         * @brief thread for sending PIR and BME statuses routinely
        */
        static void packetSender(void* pvParameter);

        /**
         * @brief Task to read the buffer of incoming EspNow data 
        */
        static void packetReader(void* pvParameter);

        static void initI();

        static void loopI();

        Vent() {}
    public:
        Vent(const Vent&) = delete;
        Vent(Vent&&) = delete;
        Vent& operator=(const Vent&) = delete;
        Vent& operator=(Vent&&) = delete;

        /**
         * @brief get instance of Vent singleton
         * @return vent
        */
        static Vent& get() { static Vent vent; return vent; }

        /**
         * @brief open vent
        */
        static void open();
        
        /**
         * @brief close vent
        */
        static void close();

        /**
         * @brief initialize vent control system (start UART--NX Display, I2C--BME, Servo, PIR) called by app_main()
        */
        static void init() { get().initI(); }

        /**
         * @brief loop called from app_main()
        */
        static void loop() { get().loopI(); }
};