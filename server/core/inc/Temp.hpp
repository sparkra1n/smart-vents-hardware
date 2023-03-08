/*
 * Title   : header file for temp conversions
 * File    : temp.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

namespace temp
{
    constexpr double cToF(double);
    constexpr double fToC(double);

    /**
     * @brief convert celsius to fahrenheit
     * @param c celsius
    */
    constexpr double cToF(double c)
    {
        return ((9 / 5) * c) + 32;
    }

    /**
     * @brief convert fahrenheit to celsius
     * @param f fahrenheit
    */
    constexpr double fToC(double f)
    {
        return (5 / 9) * (f - 32);
    }
}