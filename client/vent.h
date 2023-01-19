/*
 * File     : vent.h
 * Authors  : Ani
 * Date     : 1/6/23
*/

#pragma once

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
        vent(double temperature, double openDuration) : temp(temperature), heatingTime(openDuration) {}
        void setTemp(const double& t);
        void setOpenDuration(const double& d);
        double getTemp() const;
        double getHeatingTime() const;
        void open();
        void close();
};
