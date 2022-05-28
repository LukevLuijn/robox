// ======================================
// Filename:    LinearActuator.cpp
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#include "device/LinearActuator.h"
#include "config/config.h"
#include "utils/Time.h"

namespace Device
{
    LinearActuator::LinearActuator(uint8_t stp, uint8_t dir, uint8_t enb, uint8_t limit, Microstepping_e microstepping,
                                   float reduction, bool inverted)
        : Actuator(stp, dir, enb, microstepping, reduction, inverted),
          m_stepsPerMillimeter(m_stepsPerRevolution / LEADSCREW_PITCH), m_limitPin(limit),
          m_switch(Device::HallSensor(limit)), m_homingState(HomingState_e::SETUP), m_homingTimer(0), m_homingIndex(0)
    {
    }
    void LinearActuator::MoveDistance(float distance)
    {
        float steps = distance * m_stepsPerMillimeter;
        move(steps);
    }
    void LinearActuator::RunDistance(float distance)
    {
        float steps = distance * m_stepsPerMillimeter;
        runToNewPosition(currentPosition() + steps);
    }
    void LinearActuator::MoveToHeight(float height)
    {
        float distance = height - GetCurrentHeight();
        float steps =-1 * (distance * m_stepsPerMillimeter);
        move(steps); 
    }
    float LinearActuator::GetCurrentHeight()
    {
        return GetHeightForPosition(currentPosition());
    }
    float LinearActuator::GetDistanceToTarget()
    {
        return distanceToGo() / m_stepsPerMillimeter;
    }
    void LinearActuator::HomingSequence(float homingSpeed, float homingAccel, float speed, float accel )
    {
        const uint32_t INTERVAL_FAST = 300, INTERVAL_SLOW = 1000;

        switch (m_homingState)
        {
            case HomingState_e::SETUP:
            {
                m_switch.Start();
                setMaxSpeed(homingSpeed);
                setAcceleration(homingAccel);

                m_homingIndex = 1;
                m_homingState = HomingState_e::FIRST_PASS;
            }
            case HomingState_e::FIRST_PASS:
            {
                if (digitalRead(m_limitPin))
                {
                    setCurrentPosition(0);
                    setMaxSpeed(homingSpeed);
                    setAcceleration(homingAccel);

                    m_homingIndex = 0;
                    m_homingState = HomingState_e::SECOND_PASS;
                }
                else if (Utils::MicroTimer(m_homingTimer, INTERVAL_FAST))
                {
                    moveTo(m_homingIndex);
                    m_homingIndex++;
                    run();
                }
            }
            break;
            case HomingState_e::SECOND_PASS:
            {
                if (m_switch.Trigger())
                {
                    stop();
                    setCurrentPosition(0);
                    setMaxSpeed(speed);
                    setAcceleration(accel);
                    moveTo(0);

                    m_homingState = HomingState_e::HOMING_COMPLETE;
                }
                else if (Utils::MicroTimer(m_homingTimer, INTERVAL_SLOW))
                {
                    moveTo(m_homingIndex);
                    m_homingIndex--;
                    run();
                }
            }
            break;
            case HomingState_e::HOMING_COMPLETE:
            default:
                break;
        }
    }
    bool LinearActuator::HomingComplete()
    {
        return m_homingState == HomingState_e::HOMING_COMPLETE;
    }
    void LinearActuator::ResetHoming()
    {
        m_homingState = HomingState_e::SETUP;
    }
    float LinearActuator::GetStepsPerMillimeter()
    {
        return m_stepsPerMillimeter;
    }
    float LinearActuator::GetPosition()
    {
        return GetCurrentHeight();
    }
    float LinearActuator::GetHeightForPosition(float position)
    {
        float height = (MAX_HEIGHT_SEGMENT_00 - (position / m_stepsPerMillimeter)) - GRIPPER_HEIGHT_OFFSET +
                       FRAME_HEIGHT_OFFSET;
        return height;
    }

}// namespace Device