/*
    https://embeddedtutorials.com/eps32/esp-idf-weather-station-with-bme280-cpp/
*/

#pragma once

#include "Bme.hpp"
#include "I2c.hpp"

class BME280I2C : public BME280
{
    private:
        I2c *i2c;
        uint8_t _devAddress{};

    protected:
        esp_err_t writeByteData(const uint8_t reg, const uint8_t value);
        int readByteData(const uint8_t reg);
        int readWordData(const uint8_t reg);
        esp_err_t readBlockData(const uint8_t reg, uint8_t *buf, const int length);

    public:
        void InitI2c(I2c *i_i2c, const uint8_t dev_addr = 0x76);
};