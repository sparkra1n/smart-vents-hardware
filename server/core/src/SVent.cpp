#include "../inc/SVent.hpp"

esp_err_t SVent::open()
{ 
    Message msg{}; 
    msg.action = 1; 
    if (EspNow::send(_mac, msg) != ESP_OK)
        return ESP_FAIL;
    
    SVent::_isOpen = true;
    return ESP_OK;
}

esp_err_t SVent::close()
{ 

    Message msg{}; 
    msg.action = 0; 
    if (EspNow::send(_mac, msg) != ESP_OK)
        return ESP_FAIL;

    SVent::_isOpen = false;
    return ESP_OK;
}
