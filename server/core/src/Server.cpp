/*
 * Title   : Source File for Server
 * File    : Server.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#include "Server.hpp"

#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/event_groups.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_spiffs.h"
#include "driver/gpio.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip/netdb.h"

extern int wifi_connect_status;

#define EXAMPLE_ESP_MAXIMUM_RETRY 5

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1


static int s_retry_num = 0;
int wifi_connect_status = 0;

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI("Websocket", "retry to connect to the AP");
        }
        else
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        wifi_connect_status = 0;
        ESP_LOGI("Websocket", "connect to the AP fail");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI("Websocket", "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        wifi_connect_status = 1;
    }
}

void ws_connectWifi()
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "ASHW8",
            .password = "M0ntana0r0",
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
            .threshold = {
                .authmode = WIFI_AUTH_WPA2_PSK,
            }
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI("Websocket", "wifi_init_sta finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI("Websocket", "connected to ap ");
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI("Websocket", "Failed to connect");
    }
    else
    {
        ESP_LOGE("Websocket", "UNEXPECTED EVENT");
    }

    vEventGroupDelete(s_wifi_event_group);
}


// End header file

httpd_handle_t server = NULL;

struct async_resp_arg_t
{
    httpd_handle_t hd;
    int fd;
};

int led_state = 0;

#define INDEX_HTML_PATH "/spiffs/index.html"
char index_html[7000];
// char response_data[8192];

static void initi_web_page_buffer()
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };

    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&conf));

    memset((void *)index_html, 0, sizeof(index_html));
    struct stat st;
    if (stat(INDEX_HTML_PATH, &st))
    {
        ESP_LOGE("Websocket", "index.html not found");
        return;
    }

    FILE *fp = fopen(INDEX_HTML_PATH, "r");
    if (fread(index_html, st.st_size, 1, fp) == 0)
    {
        ESP_LOGE("Websocket", "fread failed");
    }
    fclose(fp);
}

esp_err_t get_req_handler(httpd_req_t* req)
{
    return httpd_resp_send(req, index_html, HTTPD_RESP_USE_STRLEN);
}

static void ws_async_send(void* arg)
{
    httpd_ws_frame_t ws_pkt;
    async_resp_arg_t* resp_arg = (async_resp_arg_t*) arg;
    httpd_handle_t hd = resp_arg->hd;
    int fd = resp_arg->fd;
    float room1Temp{};
    float room2Temp{};
    for (auto& vent : Server::s_ventList)
    {
        if (!strcmp(vent.getRoomName(), "room 1"))
            room1Temp = vent.getTemp();
        else if (!strcmp(vent.getRoomName(), "room 2"))
            room2Temp = vent.getTemp();
    }
    char buff[32];
    memset(buff, 0, sizeof(buff));

    sprintf(buff, "%.2f,%.2f", room1Temp, room2Temp);

    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));

    ws_pkt.payload = (uint8_t*) buff;
    ws_pkt.len = strlen(buff);
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;
    
    static size_t max_clients = CONFIG_LWIP_MAX_LISTENING_TCP;
    size_t fds = max_clients;
    int client_fds[max_clients];

    esp_err_t ret = httpd_get_client_list(server, &fds, client_fds);

    if (ret != ESP_OK) {
        return;
    }

    for (int i = 0; i < fds; i++) {
        int client_info = httpd_ws_get_fd_info(server, client_fds[i]);
        if (client_info == HTTPD_WS_CLIENT_WEBSOCKET) {
            httpd_ws_send_frame_async(hd, client_fds[i], &ws_pkt);
        }
    }
    free(resp_arg);
}

static esp_err_t trigger_async_send(httpd_handle_t handle, httpd_req_t* req)
{
    async_resp_arg_t* resp_arg = (async_resp_arg_t*) malloc(sizeof(async_resp_arg_t));
    resp_arg->hd = req->handle;
    resp_arg->fd = httpd_req_to_sockfd(req);
    return httpd_queue_work(handle, ws_async_send, resp_arg);
}

static esp_err_t handle_ws_req(httpd_req_t* req)
{
    if (req->method == HTTP_GET)
    {
        ESP_LOGI("Websocket", "Handshake done, the new connection was opened");
        return ESP_OK;
    }

    httpd_ws_frame_t ws_pkt;
    uint8_t* buf = NULL;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;
    esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
    if (ret != ESP_OK)
    {
        ESP_LOGE("Websocket", "httpd_ws_recv_frame failed to get frame len with %d", ret);
        return ret;
    }

    if (ws_pkt.len)
    {
        buf = (uint8_t*) calloc(1, ws_pkt.len + 1);
        if (buf == NULL)
        {
            ESP_LOGE("Websocket", "Failed to calloc memory for buf");
            return ESP_ERR_NO_MEM;
        }
        ws_pkt.payload = buf;
        ret = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
        if (ret != ESP_OK)
        {
            ESP_LOGE("Websocket", "httpd_ws_recv_frame failed with %d", ret);
            free(buf);
            return ret;
        }
        ESP_LOGI("Websocket", "Got packet with message: %s", ws_pkt.payload);
    }

    ESP_LOGI("Websocket", "frame len is %d", ws_pkt.len);
    //FIXME:
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT && strcmp( (char*) ws_pkt.payload, "temp") == 0)
    {
        free(buf);
        return trigger_async_send(req->handle, req);
    }
    free(buf);
    return ESP_OK;
}

httpd_handle_t setup_websocket_server(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    httpd_uri_t uri_get = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = get_req_handler,
        .user_ctx = NULL
    };

    httpd_uri_t ws = {
        .uri = "/ws",
        .method = HTTP_GET,
        .handler = handle_ws_req,
        .user_ctx = NULL,
        .is_websocket = true
    };

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &ws);
    }

    return server;
}

void Server::packetReader(void* pvParameter)
{
    while(1)
    {
        MacAddress mac;
        Message msg;

        if (EspNow::readBuffer(mac, msg))
        {
            std::cout << "Received message from " << mac 
                    << ", temp: "                 << msg.tempc 
                    << ", motion detected: "      << msg.hasMotion
                    << ", is open: "              << msg.isOpen
                    << '\n';

            for (auto& vent : Server::s_ventList)
            {
                if (vent.getMac() == mac)
                {
                    vent.setVals(msg.tempc, msg.hasMotion, msg.isOpen);
                    break;
                }
            }
        }

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

void Server::initI()
{
    esp_err_t status = nvs_flash_init();
    if (status == ESP_ERR_NVS_NO_FREE_PAGES || status == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        status = nvs_flash_init();
    }
    ESP_ERROR_CHECK(status);

    ws_connectWifi();

    if (wifi_connect_status)
    {
        ESP_LOGI("Websocket", "ESP32 ESP-IDF WebSocket Web Server is running......\n");
        initi_web_page_buffer();
        setup_websocket_server();
    }


    ESP_ERROR_CHECK(EspNow::init());

    Server::s_ventList.emplace_back( SVent("room 1", {0x70, 0x04, 0x1d, 0xfb, 0xde, 0x48}) );
    Server::s_ventList.emplace_back( SVent("room 2", {0x70, 0x04, 0x1d, 0xfb, 0xdd, 0xe4}) );

    for (auto& vent : Server::s_ventList)
        ESP_ERROR_CHECK(EspNow::addPeer(vent.getMac()));

    xTaskCreate(packetReader, "packet_reader", 2048, NULL, 4, NULL);
}

void Server::loopI()
{
    for (auto& vent : Server::s_ventList)
    {

        // Close any open vents that have no motion detected and move on
        if (!vent.getMotionState())
        {
            if (vent.getOpenState())
            {
                ESP_ERROR_CHECK(vent.close());
            }
        }

        // Motion has been detected
        else
        {
            if (vent.getTemp() < Server::setTemp && !vent.getOpenState())
            {
                ESP_ERROR_CHECK(vent.open());
            }

            else if (vent.getTemp() >= Server::setTemp && vent.getOpenState())
            {
                ESP_ERROR_CHECK(vent.close());
            }
        }
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
}