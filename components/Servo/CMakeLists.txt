idf_build_get_property(target IDF_TARGET)

set(srcs "src/Servo.cpp")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS include
                    REQUIRES esp_common driver esp_timer esp_system 
)