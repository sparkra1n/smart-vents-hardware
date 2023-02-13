/*
 * Title   : header file for esp-now
 * File    : comm.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

#include <assert.h>
#include <algorithm>
#include <memory>
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_now.h"
#include "esp_mac.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_crc.h"

#if CONFIG_ESPNOW_WIFI_MODE_STATION
#define ESPNOW_WIFI_MODE WIFI_MODE_STA
#define ESPNOW_WIFI_IF   ESP_IF_WIFI_STA
#else
#define ESPNOW_WIFI_MODE WIFI_MODE_AP
#define ESPNOW_WIFI_IF   ESP_IF_WIFI_AP
#endif

class EspNow
{
    private:
        static QueueHandle_t s_example_espnow_queue;
        void nvsInit()
        {
            esp_err_t status = nvs_flash_init();
            if (status == ESP_ERR_NVS_NO_FREE_PAGES || status == ESP_ERR_NVS_NEW_VERSION_FOUND) 
            {
                ESP_ERROR_CHECK(nvs_flash_erase());
                status = nvs_flash_init();
            }
            ESP_ERROR_CHECK(status);
        }
        void wifiInit()
        {
            ESP_ERROR_CHECK(esp_netif_init());
            ESP_ERROR_CHECK(esp_event_loop_create_default());
            wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
            ESP_ERROR_CHECK(esp_wifi_init(&cfg));
            ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
            ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
            ESP_ERROR_CHECK(esp_wifi_start());
            ESP_ERROR_CHECK(esp_wifi_set_channel(CONFIG_ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE));

            #if CONFIG_ESPNOW_ENABLE_LONG_RANGE
                ESP_ERROR_CHECK(esp_wifi_set_protocol(ESPNOW_WIFI_IF, WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N | WIFI_PROTOCOL_LR));
            #endif
        }
        static void sendCallback(const uint8_t *mac_addr, esp_now_send_status_t status)
        {
            
        }
        static void receiveCallback(const uint8_t *mac_addr, const uint8_t *data, int len)
        {
            
        }
        void initImpl()
        {

            ESP_ERROR_CHECK(esp_now_init());
            ESP_ERROR_CHECK(esp_now_register_send_cb(sendCallback));
            ESP_ERROR_CHECK(esp_now_register_recv_cb(receiveCallback));
            #if CONFIG_ESP_WIFI_STA_DISCONNECTED_PM_ENABLE
                ESP_ERROR_CHECK( esp_now_set_wake_window(65535) );
            #endif
            ESP_ERROR_CHECK( esp_now_set_pmk((uint8_t*)CONFIG_ESPNOW_PMK) );

            {
                // Add broadcast peer information to peer list
                std::unique_ptr<esp_now_peer_info_t> peer = 
                std::make_unique<esp_now_peer_info_t>((esp_now_peer_info_t) {
                    .peer_addr = {0x70, 0x04, 0x1d, 0xfb, 0xde, 0x48},
                    .channel = CONFIG_ESPNOW_CHANNEL,
                    .ifidx = WIFI_IF_STA,
                    .encrypt = false
                });
                ESP_ERROR_CHECK(esp_now_add_peer(peer.get()));
            }

            {
                    /* Initialize sending parameters. */
    send_param = malloc(sizeof(example_espnow_send_param_t));
    if (send_param == NULL) {
        ESP_LOGE(TAG, "Malloc send parameter fail");
        vSemaphoreDelete(s_example_espnow_queue);
        esp_now_deinit();
        return ESP_FAIL;
    }
    memset(send_param, 0, sizeof(example_espnow_send_param_t));
    send_param->unicast = false;
    send_param->broadcast = true;
    send_param->state = 0;
    send_param->magic = esp_random();
    send_param->count = CONFIG_ESPNOW_SEND_COUNT;
    send_param->delay = CONFIG_ESPNOW_SEND_DELAY;
    send_param->len = CONFIG_ESPNOW_SEND_LEN;
    send_param->buffer = malloc(CONFIG_ESPNOW_SEND_LEN);
    if (send_param->buffer == NULL) {
        ESP_LOGE(TAG, "Malloc send buffer fail");
        free(send_param);
        vSemaphoreDelete(s_example_espnow_queue);
        esp_now_deinit();
        return ESP_FAIL;
    }
    memcpy(send_param->dest_mac, s_example_broadcast_mac, ESP_NOW_ETH_ALEN);
    example_espnow_data_prepare(send_param);

    xTaskCreate(example_espnow_task, "example_espnow_task", 2048, send_param, 4, NULL);

    return ESP_OK;
            }
        }
        void send()
        {

        }
        EspNow() {}
        ~EspNow() {}
    public:
        EspNow(const EspNow&) = delete;
        EspNow(EspNow&&) = delete;
        EspNow& operator=(const EspNow&) = delete;
        EspNow& operator=(EspNow&&) = delete;
        static void init()
        {
            get().nvsInit();
            get().wifiInit();
            get().initImpl();
        }
        static EspNow& get() 
        { 
            static EspNow espNow;
            return espNow; 
        }

};

