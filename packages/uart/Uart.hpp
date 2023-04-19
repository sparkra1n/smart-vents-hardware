/*
 * Title   : Header file for UART Serial Communication
 * File    : Uart.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"

class Uart
{
    private:
        static void recvTask(void* pvParameter);

        Uart() {}

    public:
        Uart(const Uart&) = delete;
        Uart(Uart&&) = delete;
        Uart& operator=(const Uart&) = delete;
        Uart& operator=(Uart&&) = delete;

        /**
         * @brief initialize UART
         * @param portNum UART port number e.g. 0, 1
         * @param tx transmit pin
         * @param rx receive pin
         * @param baudRate baud rate e.g. 115200
         * @param bufLen read buffer length
         * @return ESP_OK - Success \\
         * @return ESP_FAIL - Parameter error
        */
        static esp_err_t init(uart_port_t portNum, gpio_num_t tx, gpio_num_t rx, int baudRate, uint32_t bufLen);

        /**
         * Send message
        */
        static void send(const char* msg, uint8_t len);

        static Uart& get() noexcept { static Uart uart; return uart; }

};
