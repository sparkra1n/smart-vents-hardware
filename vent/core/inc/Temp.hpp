/*
 * Title   : header file for the BME temperature sensor and C and F conversions
 * File    : temp.hpp
 * Author  : Ani
 * Date    : 01/30/23
*/

#pragma once

class Bme
{
    private:
        // double getTempImpl();
    public:
        /**
         * @brief convert celsius to fahrenheit
         * @param c celsius
        */
        static constexpr double cToF(const double&);

        /**
         * @brief convert fahrenheit to celsius
         * @param f fahrenheit
        */
        static constexpr double fToC(const double&);

        static Bme& get()
        {
            static Bme bme;
            return bme;
        }

        constexpr Bme() {}
};





