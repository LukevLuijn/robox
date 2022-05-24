// ======================================
// Filename:    Actuator.cpp
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#include "device/Actuator.h"
#include "config/config.h"

namespace Device
{
    Actuator::Actuator(uint8_t stp, uint8_t dir, uint8_t enb, Microstepping_e microstepping, float reduction,
                       bool inverted)
        : AccelStepper(AccelStepper::DRIVER, stp, dir), m_microstepping(microstepping), m_acceleration(0)
    {
        m_stepsPerRevolution = DEFAULT_STEPS_PER_REVOLUTION * (uint8_t) m_microstepping * reduction;

        setEnablePin(enb);
        setMinPulseWidth(DRIVER_MIN_PULSE_WIDTH);
        disableOutputs();
        setPinsInverted(inverted, false, false);
    }
    void Actuator::Start()
    {
        enableOutputs();
    }
    void Actuator::Run()
    {
        run();
    }
    Microstepping_e Actuator::GetMicrostepping() const
    {
        return m_microstepping;
    }
    float Actuator::GetStepsPerRevolution() const
    {
        return m_stepsPerRevolution;
    }
    void Actuator::SetAcceleration(float acceleration)
    {
        setAcceleration(acceleration);
        m_acceleration = acceleration;
    }
    float Actuator::GetAcceleration()
    {
        return m_acceleration;
    }
}// namespace Device