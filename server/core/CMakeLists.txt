set(SOURCES 
    src/Main.cpp 
    src/Server.cpp 
    src/SVent.cpp
)
idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS "." "inc"
                        REQUIRES driver EspNow lwip esp_http_server spiffs spi_flash esp_wifi esp_system
                    )
spiffs_create_partition_image(storage ../data FLASH_IN_PROJECT)