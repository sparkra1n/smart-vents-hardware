#include "vent.h"

#include <Arduino.h>
#include <stdio.h>

class Vent
{
    private:
        unsigned char id;
        double temp;
        double heatingTime;
    public:
        Vent(double temperature, double openDuration) : id(id), temp(temperature), heatingTime(openDuration) {}

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

namespace idle
{
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
}


int main()
{
    idle::init();

    while (1)
    {

    }
    
    return 0;
}
