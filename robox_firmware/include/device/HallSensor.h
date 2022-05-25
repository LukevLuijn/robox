// ======================================
// Filename:    HallSensor.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_CONTROLLER_DEVICE_HALL_SENSOR_H
#define ROBOX_CONTROLLER_DEVICE_HALL_SENSOR_H

#include <Arduino.h>

#include "utils/CircularBuffer.h"

namespace Device
{
    class HallSensor
    {
    public:
        HallSensor(uint8_t pin);
        virtual ~HallSensor() = default;

        /**
         * @brief Start the Hall sensor instance.
         * 
         */
        void Start();
        /**
         * @brief Run the hall sensor instance.
         * 
         * @note Only call this when the GetAverage() method is used, is so call at least every SAMPLE_INTERVAL_US.
         * 
         */
        void Run();
        /**
         * @brief Check if the sensor is triggered, this method implements debouncing.
         * 
         * @return true If the sensor is triggered.
         * @return false If the sensor is NOT triggered.
         */
        bool Trigger();
        /**
         * @brief Get the raw reading of the pin.s
         * 
         * @return uint8_t Raw reading.
         */
        uint8_t GetRaw();
        /**
         * @brief Get the average reading, read from the circular buffer class.
         * 
         * @see utils/CircularBuffer.h
         * 
         * @return int16_t Average reading, returns -1 if the buffer is empty.
         */
        uint8_t GetAverage();

    private:
        uint8_t m_pin;
        int16_t m_lastSwitchState;
        int16_t m_switchState;
        int32_t m_lastDebounceTime;
        int32_t m_debounceDelay;

        Utils::CBuffer m_buffer;
        uint32_t m_sampleTimer;
    };
}// namespace Device

#endif// ROBOX_CONTROLLER_DEVICE_HALL_SENSOR_H