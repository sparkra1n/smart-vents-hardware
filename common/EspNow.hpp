/*
 * Title   : Header File for Esp-Now
 * File    : EspNow.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

#include "Mac.hpp"
#include <assert.h>
#include <algorithm>    // std::copy_n
#include <cstring>      // std::memcpy
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_now.h"
#include "esp_mac.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_random.h"
#include "esp_crc.h"
#include "sdkconfig.h"

#define ESPNOW_MAXDELAY 512
#define ESPNOW_WIFI_MODE WIFI_MODE_STA
#define ESPNOW_WIFI_IF   ESP_IF_WIFI_STA
#define ESPNOW_QUEUE_SIZE 6

typedef struct 
{
    uint8_t task;
    uint8_t action;
} Message;

typedef struct
{
    uint8_t mac_addr[ESP_NOW_ETH_ALEN];
    esp_now_send_status_t status;
} espnow_event_send_cb_t;

typedef struct 
{
    uint8_t mac_addr[ESP_NOW_ETH_ALEN];
    uint8_t* data;
    int data_len;
} espnow_event_recv_cb_t;

typedef union /* When ESPNOW sending or receiving callback function is called, post event to ESPNOW task. */
{
    espnow_event_send_cb_t send_cb;
    espnow_event_recv_cb_t recv_cb;
} espnow_event_info_t;

// Parameters of sending ESPNOW data
typedef struct 
{
    Message message;                      // Message to be put into payload
    uint32_t magic;                       //Magic number which is used to determine which device to send unicast ESPNOW data.
    uint16_t count;                       //Total count of unicast ESPNOW data to be sent.
    uint16_t delay;                       //Delay between sending two ESPNOW data, unit: ms.
    int len;                              //Length of ESPNOW data to be sent, unit: byte.
    uint8_t* buffer;                      //Buffer pointing to ESPNOW data.
    uint8_t dest_mac[ESP_NOW_ETH_ALEN];   //MAC address of destination device.
} espnow_send_param_t;

// ESPNOW data field
typedef struct 
{
    uint16_t crc;                         //CRC16 value of ESPNOW data.
    uint32_t magic;                       //Magic number which is used to determine which device to send unicast ESPNOW data.
    Message msg;                          //Real payload of ESPNOW data.
} __attribute__((packed)) espnow_data_t;

typedef void(*UsrRecvCb)(const Message&);

class EspNow
{
    private:
        EspNow() {}
        static void sendCallback(const uint8_t* mac_addr, esp_now_send_status_t status);
        static void recvCallback(const uint8_t* mac_addr, const uint8_t* data, int len);
        static void prepareData(espnow_send_param_t* send_param);
        static void sendTask(void* pvParameter);
        static void recvTask(void* pvParameter);
        static void deInit(espnow_send_param_t* send_param);
        static esp_err_t addPeerImpl(const MacAddress& mac);
        // static esp_err_t remPeerImpl(const MacAddress& mac);
        static esp_err_t sendImpl(const MacAddress& mac, const Message& message);

    public:
        EspNow(const EspNow&) = delete;
        EspNow(EspNow&&) = delete;
        EspNow& operator=(const EspNow&) = delete;
        EspNow& operator=(EspNow&&) = delete;
        static EspNow& get() noexcept { static EspNow espNow; return espNow; }
        static void attachRecvCb(UsrRecvCb);
        [[__nodiscard__]] static esp_err_t init();
        [[__nodiscard__]] static esp_err_t addPeer(const MacAddress& mac) { return get().addPeerImpl(mac); }
        // [[__nodiscard__]] static esp_err_t remPeer(const MacAddress& mac) { return get().remPeerImpl(mac); }
        [[__nodiscard__]] static esp_err_t send(const MacAddress& mac, const Message& message) { return get().sendImpl(mac, message); }
};

