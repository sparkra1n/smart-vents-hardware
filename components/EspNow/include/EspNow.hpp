/*
 * Title   : Header File for Esp-Now
 * File    : EspNow.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

#include "Mac.hpp"

#include <iostream>
#include <assert.h>
#include <algorithm> // std::copy, std::copy_n

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

#define ESPNOW_QUEUE_SIZE 16

struct Message
{
    // For Vent
    uint8_t action;                             // Open or close the vent

    // For Server 
    float tempc;                                // Temperature in C
    bool hasMotion;                             // Whether or not motion has been detected
    bool isOpen;                                // Vent's open/close state
};

struct espnow_event_send_cb_t
{
    uint8_t mac_addr[ESP_NOW_ETH_ALEN];
    esp_now_send_status_t status;
};

struct espnow_event_recv_cb_t
{
    uint8_t mac_addr[ESP_NOW_ETH_ALEN];
    uint8_t* data;
    int data_len;
};

// When ESPNOW sending or receiving callback function is called, post event to ESPNOW task.
union espnow_event_info_t
{
    espnow_event_send_cb_t send_cb;
    espnow_event_recv_cb_t recv_cb;
};

// Parameters of sending ESPNOW data
struct espnow_send_param_t
{
    Message message;                            // Message to be put into payload
    uint32_t magic;                             //Magic number which is used to determine which device to send unicast ESPNOW data.
    uint16_t count;                             //Total count of unicast ESPNOW data to be sent.
    uint16_t delay;                             //Delay between sending two ESPNOW data, unit: ms.
    int len;                                    //Length of ESPNOW data to be sent, unit: byte.
    uint8_t* buffer;                            //Buffer pointing to ESPNOW data.
    uint8_t dest_mac[ESP_NOW_ETH_ALEN];         //MAC address of destination device.
};

// ESPNOW data field
struct espnow_data_t
{
    uint16_t crc;                               //CRC16 value of ESPNOW data.
    uint32_t magic;                             //Magic number which is used to determine which device to send unicast ESPNOW data.
    Message msg;                                //Real payload of ESPNOW data.
} __attribute__((packed));

class EspNow
{
    private:
        EspNow() {}

        /**
         * @brief send callback from espnow.h that places an event to the send queue
        */
        static void sendCallback(const uint8_t* mac_addr, esp_now_send_status_t status);

        /**
         * @brief receive callback from espnow.h that places an event in the recv queue 
        */
        static void recvCallback(const uint8_t* mac_addr, const uint8_t* data, int len);

        /**
         * @brief prepares espnow_send_param for sending
         * @param send_param struct for send parameters 
        */
        static void prepareData(espnow_send_param_t* send_param);

        /**
         * @brief FreeRTOS send task
        */
        static void sendTask(void* pvParameter);

        static void deInitI(espnow_send_param_t* send_param) noexcept;

        static esp_err_t initI();

        static esp_err_t sendI(const MacAddress& mac, const Message& msg, uint8_t count);
        
        static esp_err_t addPeerI(const MacAddress& mac);

        static esp_err_t remPeerI(const MacAddress& mac);

        static bool readI(MacAddress& mac, Message& msg);

    public:
        EspNow(const EspNow&) = delete;
        EspNow(EspNow&&) = delete;
        EspNow& operator=(const EspNow&) = delete;
        EspNow& operator=(EspNow&&) = delete;

        /**
         * @brief returns single instance of EspNow
        */
        static EspNow& get() noexcept { static EspNow espNow; return espNow; }

        /**
         * @brief deinit esp_now and deletes send/recieve queues
        */
        static void deInit(espnow_send_param_t* send_param) noexcept { get().deInitI(send_param); }

        /**
         * @brief initialize queues for sending and receiving 
        */
        [[__nodiscard__]] static esp_err_t init() { return get().initI(); }

        /**
         * @brief send message to a specific device
         * @param mac recipient mac address
         * @param msg message struct to be sent
         * @param count number of times to repeat sending that message
        */
        [[__nodiscard__]] static esp_err_t send(const MacAddress& mac,
                                                const Message& msg,
                                                uint8_t count=1) 
                            { return get().sendI(mac, msg, count); }
        
        /**
         * @brief add peer to peer list in order to send message to it
         * @param mac peer's mac address
        */
        [[__nodiscard__]] static esp_err_t addPeer(const MacAddress& mac) 
                            { return get().addPeerI(mac); }

        /**
         * @brief remove peer from peer list in order make space in the list's buffer 
         * @param mac peer's mac address
        */
        [[__nodiscard__]] static esp_err_t remPeer(const MacAddress& mac) 
                            { return get().remPeerI(mac); }

        /**
         * @brief read from the receive buffer queue
         * @param Message received message
         * @param MacAddress mac address of the sender
         * @return true - read successfully \\
         * @return false - empty or bad message
        */
       [[__nodiscard__]] static bool readBuffer(MacAddress& mac, Message& msg) 
                            { return get().readI(mac, msg); }
};

