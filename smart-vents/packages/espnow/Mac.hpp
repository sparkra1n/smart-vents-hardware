#pragma once

#include <iostream>
#include <iomanip>
#include <array>
#include <stdio.h>
#include "esp_now.h"

typedef std::array<uint8_t, ESP_NOW_ETH_ALEN> MacAddress;

inline std::ostream& operator<<(std::ostream& os, const MacAddress& m)
{
    os << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (0xFF & m[0]) << ":"
       << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (0xFF & m[1]) << ":"
       << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (0xFF & m[2]) << ":"
       << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (0xFF & m[3]) << ":"
       << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (0xFF & m[4]) << ":"
       << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (0xFF & m[5]);
    return os;
}