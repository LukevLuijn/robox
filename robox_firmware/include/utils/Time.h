// ======================================
// Filename:    Time.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_CONTROLLER_UTILS_TIME_H
#define ROBOX_CONTROLLER_UTILS_TIME_H

#include <Arduino.h>

namespace Utils 
{
    /**
     * @brief Microsecond timer.
     * 
     * @param time Reference to time variable, gets updated if interval is reached.
     * @param interval Interval to check for.
     * @return true If the interval is reached.
     * @return false If the interval is NOT reached.
     */
    static bool MicroTimer(uint32_t& time, uint32_t interval)
    {
        if (micros() - time >= interval)
        {
            time = micros();
            return true;
        }
        return false;
    }
}

#endif // ROBOX_CONTROLLER_UTILS_TIME_H