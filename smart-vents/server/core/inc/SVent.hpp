#pragma once

#include "../../packages/espnow/EspNow.hpp"

/**
 * @brief Vent object on the server side
*/
class SVent
{
    private:
        const char* _room;
        const MacAddress _mac;
        float _temp;
        bool _hasMotion;
        bool _isOpen;

    public:
        SVent(const char* room, const MacAddress mac) : _room(room), _mac(mac), _temp(0.0f), 
                                                        _hasMotion(false), _isOpen(false) {}

        /**
         * @brief Update the vent's temperature
         * @param temp temperature to set
        */
        void setTemp(float temp) noexcept { _temp = temp; }
        
        /**
         * @brief Set and update whether motion has been detected at this vent
         * @param hasMotion active/inactive state to set
        */
        void setMotionState(bool hasMotion) noexcept { _hasMotion = hasMotion; }

        /**
         * @brief Set and update whether this vent is open or closed
         * @param isOpen open/close state to set
        */
        void setOpenState(bool isOpen) noexcept { _isOpen = isOpen; }

        /**
         * @brief Get the last update on the vent's temperature sensor
         * @return temperature
        */        
        float getTemp() const noexcept { return _temp; }

        /**
         * @brief Get the last update on the whether the vent has detected motion
         * @return true / false
        */
        bool getMotionState() const noexcept { return _hasMotion; }

        /**
         * @brief Get the last update on whether the vent is open or closed
        */
        bool getOpenState() const noexcept { return _isOpen; }

        /**
         * @brief Return the vent's Mac Address
         * @return Mac Address
        */
        MacAddress getMac() const noexcept { return _mac; }

        /**
         * @brief Set and update all the values
         * @param temp temperature to set
         * @param hasMotion active/inactive state to set
         * @param isOpen open/close state to set
        */
        void setVals(float temp, bool hasMotion, bool isOpen) noexcept 
                    { _temp = temp; _hasMotion = hasMotion; _isOpen = isOpen; }
        
        /**
         * @brief Send "open" command to this vent through ESP-NOW
         * @return ESP_OK - send success
         * @return ESP_FAIL - send fail (did you add the peer?)
        */
        [[__nodiscard__]] esp_err_t open();

        /**
         * @brief Send "close" command to this vent through ESP-NOW
         * @return ESP-OK - send success
         * @return ESP_FAIL - send fail (did you add the peer?)
        */
        [[__nodiscard__]] esp_err_t close();
};