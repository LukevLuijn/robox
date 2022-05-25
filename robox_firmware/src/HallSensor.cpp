// ======================================
// Filename:    HallSensor.cpp
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#include "device/HallSensor.h"
#include "utils/Time.h"
#include "utils/Logger.h"

namespace
{
#define SAMPLE_INTERVAL_US 100
}// namespace

namespace Device
{
    HallSensor::HallSensor(uint8_t pin) : m_pin(pin), m_sampleTimer(0)
    {
    }
    void HallSensor::Start()
    {
        pinMode(m_pin, INPUT);
        DEBUG("Setup complete", "HallSensor");
    }
    void HallSensor::Run()
    {
        if (Utils::MicroTimer(m_sampleTimer, SAMPLE_INTERVAL_US))
        {
            m_buffer.Push(digitalRead(m_pin));
        }
    }
    bool HallSensor::Trigger()
    {
        bool isTriggered = false;
        int16_t reading = digitalRead(m_pin);
        if (reading != m_lastSwitchState)
        {
            m_lastDebounceTime = millis();
        }

        if ((static_cast<int32_t>(millis()) - m_lastDebounceTime) > m_debounceDelay)
        {
            if (reading != m_switchState)
            {
                m_switchState = reading;
                if (m_switchState == LOW)
                {
                    isTriggered = true;
                }
            }
        }
        m_lastSwitchState = reading;
        return isTriggered;
    }
    uint8_t HallSensor::GetRaw()
    {
        return digitalRead(m_pin);
    }
    uint8_t HallSensor::GetAverage()
    {
        float average = m_buffer.GetAverage();
        return (uint8_t)round(average);
    }
}// namespace Device