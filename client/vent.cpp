/*
 * File     : vent.cpp
 * Authors  : Ani
 * Date     : 1/6/23
*/

#include "vent.h"
#include "../payload.h"

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#include <algorithm>
#include <string>
#include <vector>
#include <array>
#include <stdio.h>

class Vent
{
    private:
        double temp;
        double heatingTime;
    public:
        Vent(double temperature, double openDuration) : temp(temperature), heatingTime(openDuration) {}

        void setTemp(const double& t)
        {
            temp = t;
        }

        void setOpenDuration(const double& d)
        {
            heatingTime = d;
        }

        double getTemp() const
        {
            return temp;
        }

        double getHeatingTime() const
        {
            return heatingTime;
        }

        void open()
        {
            
        }

        void close()
        {

        }
};
/*
    hw_timer_t *timerInterrupt = NULL;

    void init()
    {
        timerInterrupt = timerBegin(0, 80, true);
        timerAttachInterrupt(timerInterrupt, &onTimer, true);
        timerAlarmWrite(timerInterrupt, 1000000, true);
        timerAlarmEnable(timerInterrupt);
    }

    void IRAM_ATTR onTimer()
    {
        // do sensor stuff
    }
    */