#include "vent.h"

#include <Arduino.h>
#include <stdio.h>


class vent
{
    private:
        double temp;
        double heatingTime;
    public:
        vent(double temperature, double openDuration) : temp(temperature), heatingTime(openDuration) {}

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


hw_timer_t *timerInterrupt = NULL;

void IRAM_ATTR onTimer()
{
    digitalWrite(LED, !digitalRead(LED));
}

void setup() 
{
    timerInterrupt = timerBegin(0, 80, true);
    timerAttachInterrupt(timerInterrupt, &onTimer, true);
    timerAlarmWrite(timerInterrupt, 1000000, true);
    timerAlarmEnable(timerInterrupt); //Just Enable
}

void loop() 
{

}
