/*
 * Title   : header file for the vent side
 * File    : Vent.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

#include "../../packages/espnow/EspNow.hpp"
#include "../../packages/servo/Servo.hpp"
#include "../../packages/uart/Uart.hpp"
#include "../../packages/pir/Pir.hpp"
#include "../../packages/bme/Bme.hpp"
#include "../../packages/bme/BmeI2c.hpp"


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

constexpr int I2C_SDA = 2;
constexpr int I2C_SCL = 3;
constexpr uint32_t I2C_CLK_SPEED_HZ = 400000;
constexpr MacAddress SERVER = { 0x70, 0x04, 0x1d, 0xfb, 0xde, 0x30 };

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
        static void init();

        /**
         * @brief loop called from app_main()
        */
        static void loop();
};