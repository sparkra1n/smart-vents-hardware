set(SOURCES 
                    ./src/Main.cpp 
                    ../../packages/espnow/EspNow.cpp 
                    ../../packages/uart/Uart.cpp 
                    ./src/Server.cpp 
                    ./src/SVent.cpp
                    )
idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS "."
                        REQUIRES driver esp_wifi esp_common nvs_flash esp_timer
                    )
