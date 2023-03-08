/*
 * Title   : source file for the BME temperature sensor and C and F conversions
 * File    : temp.cpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#include "../inc/temp.hpp"

#include <stdio.h>

constexpr double Bme::cToF(const double& c) { return ((9 / 5) * c) + 32; }
constexpr double Bme::fToC(const double& f) { return (5 / 9) * (f - 32); }

// double Bme::getTempImpl()
// {

// }