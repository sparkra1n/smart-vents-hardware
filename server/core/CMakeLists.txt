set(SOURCES ./src/Main.cpp ../../common/EspNow.cpp ./src/Server.cpp ./src/SxVent.cpp)
idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS "."
                        REQUIRES driver esp_wifi esp_common nvs_flash
                    )
