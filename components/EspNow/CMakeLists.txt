idf_build_get_property(target IDF_TARGET)

set(srcs "src/EspNow.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS include
                    REQUIRES esp_common driver esp_wifi esp_common nvs_flash esp_timer esp_system
)