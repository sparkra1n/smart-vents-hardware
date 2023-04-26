#include "Pir.hpp"

void Pir::pirTask(void* pvParameter)
{
    while (1)
    {
        for (Pir* p : Pir::s_pirs)
        {
            uint64_t nowTime = esp_timer_get_time();

            // Switch status to true if new motion is detected 
            if (gpio_get_level(p->_pin))
            {
                // Debounce
                if ((nowTime - p->_lastTime) > 5 * 1000000) // 5s in us
                {
                    std::cout << "(re)activated on pin " << p->_pin << '\n'; 
                    p->_status = true;
                    p->_lastTime = nowTime;
                }
            }
            
            else
            {
                // Change status to false if motion has not been detected in 20s and status used to be high
                if (((nowTime - p->_lastTime) > 20 * 1000000) && p->_status) // 20s in us
                {
                    std::cout << "deactivated on pin " << p->_pin << '\n'; 
                    p->_status = false;
                }
            }
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void Pir::init()
{
    xTaskCreate(pirTask, "gptimer", 2048, NULL, 1, NULL);
}

Pir::Pir(gpio_num_t pin) : _pin(pin) 
{
    gpio_set_direction(pin, GPIO_MODE_INPUT);
    Pir::s_pirs.emplace_back(this);
}
