#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "sdkconfig.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#pragma once

#define ESP_WIFI_SSID      "kat"
#define ESP_WIFI_PASS      "kitkatclub"
#define ESP_WIFI_CHANNEL   1
#define MAX_STA_CONN       4

class Wifi
{
    private:
        Wifi() {}
        esp_err_t initI();

    public:
        static Wifi& get() { static Wifi wifi; return wifi; }

        [[__nodiscard__]] esp_err_t init() { return get().initI(); }
};