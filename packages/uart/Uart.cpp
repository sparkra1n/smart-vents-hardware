/*
 * Title   : Source file for UART Serial Communication
 * File    : Uart.cpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#include "Uart.hpp"

// constexpr static const char* TAG = "Uart.cpp";

static uint8_t s_port;
static uint16_t s_bufLen;
static QueueHandle_t s_uart_RecvQueue;

void Uart::recvTask(void* pvParameter)
{
    // ESP_LOGI("Uart.cpp", "called new");
    uint8_t* data = new uint8_t[s_bufLen];
    while (1)
    {
        // printf("ran\n");
        int len = uart_read_bytes(s_port, data, (s_bufLen - 1), 20 / portTICK_PERIOD_MS);
        if (len) 
        {
            data[len] = '\0';
            printf("Recv str: %s\n", (char*) data);
        }
    }
    delete[] data;
}

esp_err_t Uart::init(uart_port_t port, gpio_num_t tx, gpio_num_t rx, int baudRate, uint32_t bufLen)
{
    s_port = port;
    s_bufLen = bufLen;

    uart_config_t uart_config;
    uart_config.baud_rate  = baudRate;
    uart_config.data_bits  = UART_DATA_8_BITS;
    uart_config.parity     = UART_PARITY_DISABLE;
    uart_config.stop_bits  = UART_STOP_BITS_1;
    uart_config.flow_ctrl  = UART_HW_FLOWCTRL_DISABLE;
    uart_config.source_clk = UART_SCLK_DEFAULT;
    
    #if CONFIG_UART_ISR_IN_IRAM
        intr_alloc_flags = ESP_INTR_FLAG_IRAM;
    #endif

    if (uart_driver_install(port, bufLen * 2, 0, 0, NULL, 0) != ESP_OK) 
        return ESP_FAIL;
    if (uart_param_config(port, &uart_config) != ESP_OK) 
        return ESP_FAIL;
    if (uart_set_pin(port, tx, rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE) != ESP_OK) 
        return ESP_FAIL;
    
    s_uart_RecvQueue = xQueueCreate(16, s_bufLen * sizeof(char));
    
    xTaskCreate(recvTask, "uart_task", 2048, NULL, 10, NULL);
    return ESP_OK;
}

void Uart::send(const char* msg, uint8_t len)
{
    uart_write_bytes(s_port, msg, len);
}


