set(SOURCES src/Main.cpp 
            src/Vent.cpp
)
idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS "." "inc"
                        REQUIRES driver
                                 Servo
                                 Bme
                                 I2c
                                 EspNow
                                 Pir
)
