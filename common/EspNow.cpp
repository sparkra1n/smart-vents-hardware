/*
 * Title   : Source File for Esp-Now
 * File    : EspNow.cpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#include "EspNow.hpp"

constexpr static auto TAG = "EspNow";

static QueueHandle_t s_sendQueue = NULL;
static QueueHandle_t s_recvQueue = NULL;
static UsrRecvCb usrRecvCb = NULL;

void EspNow::sendCallback(const uint8_t* mac_addr, esp_now_send_status_t status)
{
    espnow_event_info_t evt;
    espnow_event_send_cb_t* send_cb = &evt.send_cb;

    if (mac_addr == NULL) 
    {
        ESP_LOGE(TAG, "Send cb arg error");
        return;
    }

    std::memcpy(send_cb->mac_addr, mac_addr, ESP_NOW_ETH_ALEN);
    send_cb->status = status;

    if (xQueueSend(s_sendQueue, &evt, ESPNOW_MAXDELAY) != pdTRUE) 
    {
        ESP_LOGW(TAG, "Send send queue fail");
    }
}

void EspNow::recvCallback(const uint8_t* mac_addr, const uint8_t* data, int len)
{
    espnow_event_info_t evt;
    espnow_event_recv_cb_t *recv_cb = &evt.recv_cb;

    if (mac_addr == NULL || data == NULL || len <= 0) 
    {
        ESP_LOGE(TAG, "Receive cb arg error");
        return;
    }

    std::memcpy(recv_cb->mac_addr, mac_addr, ESP_NOW_ETH_ALEN);
    recv_cb->data = new uint8_t[len];
    if (recv_cb->data == NULL) 
    {
        ESP_LOGE(TAG, "New receive data fail");
        return;
    }

    std::memcpy(recv_cb->data, data, len);
    recv_cb->data_len = len;
    if (xQueueSend(s_recvQueue, &evt, ESPNOW_MAXDELAY) != pdTRUE) 
    {
        ESP_LOGW(TAG, "Send receive queue fail");
        delete[] recv_cb->data;
    }
}

void EspNow::prepareData(espnow_send_param_t* send_param)
{
    espnow_data_t *buf = (espnow_data_t*) send_param->buffer;
    assert(send_param->len >= sizeof(espnow_data_t));
    // buf->state = send_param->state;
    buf->crc = 0;
    buf->magic = send_param->magic;
    buf->msg = send_param->message;
    buf->crc = esp_crc16_le(UINT16_MAX, (uint8_t const*) buf, send_param->len);
}

void EspNow::sendTask(void* pvParameter)
{
    espnow_event_info_t evt;
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "Start sending data");

    // Start sending ESPNOW data
    espnow_send_param_t* send_param = static_cast<espnow_send_param_t*>(pvParameter);
    if (esp_now_send(send_param->dest_mac, send_param->buffer, send_param->len) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Send error");
        deInit(send_param);
        vTaskDelete(NULL);
    }

    while (xQueueReceive(s_sendQueue, &evt, 500) == pdTRUE)
    {
        espnow_event_send_cb_t *send_cb = &evt.send_cb;
        
        send_param->count--;
        if (send_param->count == 0) 
        {
            ESP_LOGI(TAG, "send data to "MACSTR" done", MAC2STR(send_cb->mac_addr));
            delete[] send_param->buffer;
            delete send_param;
            vTaskDelete(NULL);
        }

        // Delay before sending next payload if there is a delay
        if (send_param->delay > 0)
            vTaskDelay(send_param->delay / portTICK_PERIOD_MS);

        ESP_LOGI(TAG, "sending data to "MACSTR"", MAC2STR(send_cb->mac_addr));
        prepareData(send_param);

        // Send the next payload after the previous one is sent
        if (esp_now_send(send_param->dest_mac, send_param->buffer, send_param->len) != ESP_OK) 
        {
            ESP_LOGE(TAG, "Send error");
            deInit(send_param);
            vTaskDelete(NULL);
        }
    }
}

void EspNow::recvTask(void* pvParameter)
{
    espnow_event_info_t evt;

    while (1)
    {
        if (xQueueReceive(s_recvQueue, &evt, 500))
        {
            espnow_event_recv_cb_t* recv_cb = &evt.recv_cb;
            espnow_data_t* buf = (espnow_data_t*) recv_cb->data;
            
            if (recv_cb->data_len < sizeof(espnow_data_t))
            {
                ESP_LOGE(TAG, "Receive ESPNOW data too short, len:%d", recv_cb->data_len);
            }

            else
            {
                ESP_LOGI(TAG, "Received data from: "MACSTR", len: %d", MAC2STR(recv_cb->mac_addr), recv_cb->data_len);
                if (!usrRecvCb)
                    ESP_LOGW(TAG, "No callback function attached with attachRecvCb()");
                else
                    usrRecvCb(buf->msg);
            }
            delete[] recv_cb->data;
        }
    }
}

esp_err_t EspNow::addPeerImpl(const MacAddress& mac)
{
    esp_now_peer_info_t peer = {
        .channel = CONFIG_ESPNOW_CHANNEL,
        .ifidx = WIFI_IF_STA,
        .encrypt = false
    };
    std::copy_n(mac.begin(), ESP_NOW_ETH_ALEN, peer.peer_addr);  
    ESP_ERROR_CHECK(esp_now_add_peer(&peer));
    return ESP_OK;
}

// esp_err_t EspNow::remPeerImpl(const MacAddress& mac)
// {

// }

esp_err_t EspNow::sendImpl(const MacAddress& mac, const Message& message)
{
    // Initialize sending parameters
    espnow_send_param_t* send_param = new espnow_send_param_t {
        .message = message,
        .magic = esp_random(),
        .count = 1,
        .delay = CONFIG_ESPNOW_SEND_DELAY,
        .len = CONFIG_ESPNOW_SEND_LEN,
    };

    if (send_param == NULL) 
    {
        ESP_LOGE(TAG, "New send parameter fail");
        vSemaphoreDelete(s_sendQueue);
        esp_now_deinit();
        return ESP_FAIL;
    }

    send_param->buffer = new uint8_t[CONFIG_ESPNOW_SEND_LEN];
    if (send_param->buffer == NULL) 
    {
        ESP_LOGE(TAG, "New send buffer fail");
        delete send_param;
        vSemaphoreDelete(s_sendQueue);
        esp_now_deinit();
        return ESP_FAIL;
    }

    std::copy_n(mac.begin(), ESP_NOW_ETH_ALEN, send_param->dest_mac);
    prepareData(send_param);
    ESP_LOGI(TAG, "creating task");
    xTaskCreate(sendTask, "send_task", 2048, send_param, 4, NULL);
    return ESP_OK;
}

void EspNow::deInit(espnow_send_param_t* send_param)
{
    delete[] send_param->buffer;
    delete send_param;
    vSemaphoreDelete(s_sendQueue);
    vSemaphoreDelete(s_recvQueue);
    esp_now_deinit();
}

void EspNow::attachRecvCb(UsrRecvCb func)
{
    usrRecvCb = func;
}

esp_err_t EspNow::init()
{
    // NVS init
    esp_err_t status = nvs_flash_init();
    if (status == ESP_ERR_NVS_NO_FREE_PAGES || status == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        status = nvs_flash_init();
    }
    ESP_ERROR_CHECK(status);

    // Wifi init
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_channel(CONFIG_ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE));

    s_sendQueue = xQueueCreate(ESPNOW_QUEUE_SIZE, sizeof(espnow_event_info_t));
    if (s_sendQueue == NULL) 
    {
        ESP_LOGE(TAG, "Create send mutex fail");
        return ESP_FAIL;
    }

    s_recvQueue = xQueueCreate(ESPNOW_QUEUE_SIZE, sizeof(espnow_event_info_t));
    if (s_recvQueue == NULL) 
    {
        ESP_LOGE(TAG, "Create recv mutex fail");
        return ESP_FAIL;
    }

    xTaskCreate(recvTask, "recv_task", 2048, NULL, 4, NULL);

    // Initialize ESPNOW and register sending and receiving callback function
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(sendCallback));
    ESP_ERROR_CHECK(esp_now_register_recv_cb(recvCallback));

    // Set primary master key
    ESP_ERROR_CHECK(esp_now_set_pmk((uint8_t*) CONFIG_ESPNOW_PMK));
    return ESP_OK;
}


