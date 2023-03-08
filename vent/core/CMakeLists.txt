set(SOURCES ./src/Main.cpp ./src/Pir.cpp ./src/Servo.cpp ./src/Temp.cpp ./src/Vent.cpp ../../common/EspNow.cpp)
idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS "."
                        REQUIRES driver esp_wifi esp_common nvs_flash esp_timer
                    )
