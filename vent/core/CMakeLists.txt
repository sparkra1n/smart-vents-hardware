set(SOURCES ./src/Main.cpp 
            ./src/Vent.cpp
            ../../packages/pir/Pir.cpp 
            ../../packages/servo/Servo.cpp 
            ../../packages/bme/Bme.cpp
            ../../packages/bme/BmeI2c.cpp
            ../../packages/i2c/I2c.cpp
            ../../packages/espnow/EspNow.cpp 
            ../../packages/uart/Uart.cpp
)
idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS "."
                        REQUIRES driver 
                                 esp_wifi 
                                 esp_common 
                                 nvs_flash 
                                 esp_timer 
                                 spi_flash
)
