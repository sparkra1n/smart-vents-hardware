idf_build_get_property(target IDF_TARGET)

set(srcs "src/I2c.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS include
                    REQUIRES driver esp_common esp_timer

)