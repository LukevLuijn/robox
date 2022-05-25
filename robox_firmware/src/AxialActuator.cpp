// ======================================
// Filename:    AxialActuator.cpp
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#include "device/AxialActuator.h"
#include "utils/Time.h"

namespace Device
{
    AxialActuator::AxialActuator(uint8_t stp, uint8_t dir, uint8_t enb, uint8_t limit, Microstepping_e microstepping,
                                 float reduction, bool inverted, HomingStyle_e style)
        : Actuator(stp, dir, enb, microstepping, reduction, inverted), m_stepsPerDegree(360.0 / m_stepsPerRevolution),
          m_limitPin(limit), m_switch(Device::HallSensor(limit)), m_homingStyle(style),
          m_homingStateDouble(HomingStateDouble_e::SETUP), m_homingStateSingle(HomingStateSingle_e::SETUP),
          m_homingTimer(0), m_homingIndex(0), m_magnetPos01(0), m_magnetPos02(0)
    {
    }
    void AxialActuator::MoveDegrees(float degrees)
    {
        float steps = degrees / m_stepsPerDegree;
        move(steps);
    }
    void AxialActuator::RunDegrees(float degrees)
    {
        float steps = degrees / m_stepsPerDegree;
        runToNewPosition(currentPosition() + steps);
    }
    void AxialActuator::MoveToAngle(float angle)
    {
        float steps = angle / m_stepsPerDegree;
        moveTo(steps);
    }
    void AxialActuator::RunToAngle(float angle)
    {
        float steps = angle / m_stepsPerDegree;
        runToNewPosition(currentPosition() + steps);
    }
    float AxialActuator::GetCurrentAngle()
    {
        float angle = currentPosition() * m_stepsPerDegree;        
        return angle;
    }
    float AxialActuator::GetDegreesToTarget()
    {
        return (float) distanceToGo() / m_stepsPerDegree;
    }
    void AxialActuator::HomingSequence(float speed, float accel)
    {
        switch (m_homingStyle)
        {
            case HomingStyle_e::SINGLE_MAGNET:
                HomeSingleMagnet(speed, accel);
                break;
            case HomingStyle_e::DOUBLE_MAGNET:
                HomeDoubleMagnet(speed, accel);
                break;
        }
    }
    bool AxialActuator::HomingComplete()
    {
        return (m_homingStyle == HomingStyle_e::SINGLE_MAGNET)
                       ? m_homingStateSingle == HomingStateSingle_e::HOMING_COMPLETE
                       : m_homingStateDouble == HomingStateDouble_e::HOMING_COMPLETE;
    }
    void AxialActuator::ResetHoming()
    {
        m_homingStateSingle = HomingStateSingle_e::SETUP;
        m_homingStateDouble = HomingStateDouble_e::SETUP;
    }
    float AxialActuator::GetStepsPerDegree()
    {
        return m_stepsPerDegree;
    }
    void AxialActuator::HomeSingleMagnet(float speed, float accel)
    {
        const uint32_t INTERVAL_FAST = 300, INTERVAL_SLOW = 300;

        switch (m_homingStateSingle)
        {
            case HomingStateSingle_e::SETUP:
            {
                m_homingIndex = 0;
                m_switch.Start();
                setMaxSpeed(speed);
                setAcceleration(accel);

                m_homingStateSingle = HomingStateSingle_e::FIRST_PASS;
            }
            case HomingStateSingle_e::FIRST_PASS:
            {
                if (digitalRead(m_limitPin))
                {
                    setCurrentPosition(0);
                    setMaxSpeed(speed);
                    setAcceleration(accel);
                    m_homingIndex = 1;
                    m_homingStateSingle = HomingStateSingle_e::SECOND_PASS;
                }
                else if (Utils::MicroTimer(m_homingTimer, INTERVAL_FAST))
                {
                    moveTo(m_homingIndex);
                    m_homingIndex--;
                    run();
                }
            }
            break;
            case HomingStateSingle_e::SECOND_PASS:
            {
                if (m_switch.Trigger())
                {
                    m_magnetPos01 = currentPosition();

                    setCurrentPosition(0);
                    setMaxSpeed(speed);
                    setAcceleration(accel);
                    MoveDegrees(30.0f);
                    m_homingStateSingle = HomingStateSingle_e::MOVING_OFFSET;
                }
                else if (Utils::MicroTimer(m_homingTimer, INTERVAL_SLOW))
                {
                    moveTo(m_homingIndex);
                    m_homingIndex++;
                    run();
                }
            }
            break;
            case HomingStateSingle_e::MOVING_OFFSET:
            {
                if (distanceToGo() > 0)
                {
                    run();
                }
                else
                {
                    m_homingIndex = currentPosition();
                    m_homingStateSingle = HomingStateSingle_e::FOURTH_PASS;
                }
            }
            break;
            case HomingStateSingle_e::FOURTH_PASS:
            {
                if (m_switch.Trigger())
                {
                    m_magnetPos02 = currentPosition() + m_magnetPos01;

                    setCurrentPosition(0);
                    setMaxSpeed(speed);
                    setAcceleration(accel);

                    float magnetDelta = m_magnetPos02 - m_magnetPos01;
                    float zeroPos = magnetDelta / 2;
                    zeroPos += (180.0f / m_stepsPerDegree);
                    move(-zeroPos);

                    m_homingStateSingle = HomingStateSingle_e::ZERO_POS_MOVE;
                }
                else if (Utils::MicroTimer(m_homingTimer, INTERVAL_SLOW))
                {
                    moveTo(m_homingIndex);
                    m_homingIndex--;
                    run();
                }
            }
            break;
            case HomingStateSingle_e::ZERO_POS_MOVE:
            {
                if (distanceToGo() != 0)
                {
                    run();
                }
                else
                {
                    stop();
                    setCurrentPosition(0);
                    setMaxSpeed(speed);
                    setAcceleration(accel);
                    moveTo(0);
                    m_homingStateSingle = HomingStateSingle_e::HOMING_COMPLETE;
                }
            }
            break;
            case HomingStateSingle_e::HOMING_COMPLETE:
            default:
                break;
        }
    }
    void AxialActuator::HomeDoubleMagnet(float speed, float accel)
    {
        const uint32_t INTERVAL_FAST = 750, INTERVAL_SLOW = 2500;

        switch (m_homingStateDouble)
        {
            case HomingStateDouble_e::SETUP:
            {
                pinMode(m_limitPin, INPUT);
                m_homingIndex = 1;
                setMaxSpeed(speed);
                setAcceleration(accel);
                
                m_homingStateDouble = HomingStateDouble_e::FM_FIRST_PASS;
            }
            case HomingStateDouble_e::FM_FIRST_PASS:
            {
                if (!digitalRead(m_limitPin))
                {
                    m_magnetPos01 = currentPosition();
                    setCurrentPosition(0);
                    setMaxSpeed(speed);
                    setAcceleration(accel);
                    m_homingIndex = 0;

                    m_homingStateDouble = HomingStateDouble_e::FM_SECOND_PASS;
                }
                else if (Utils::MicroTimer(m_homingTimer, INTERVAL_FAST))
                {
                    moveTo(m_homingIndex);
                    m_homingIndex++;
                    run();
                }
            }
            break;
            case HomingStateDouble_e::FM_SECOND_PASS:
            {
                if (digitalRead(m_limitPin))
                {
                    m_magnetPos01 += currentPosition();
                    setCurrentPosition(0);

                    setMaxSpeed(speed);
                    setAcceleration(accel);
                    MoveDegrees(-150.0f);

                    m_homingStateDouble = HomingStateDouble_e::MOVING_OFFSET;
                }
                else if (Utils::MicroTimer(m_homingTimer, INTERVAL_SLOW))
                {
                    moveTo(m_homingIndex);
                    m_homingIndex--;
                    run();
                }
            }
            break;
            case HomingStateDouble_e::MOVING_OFFSET:
            {
                if (distanceToGo() != 0)
                {
                    run();
                }
                else
                {
                    m_homingIndex = currentPosition();
                    setMaxSpeed(speed);
                    setAcceleration(accel);

                    m_homingStateDouble = HomingStateDouble_e::SM_FIRST_PASS;
                }
            }
            break;
            case HomingStateDouble_e::SM_FIRST_PASS:
            {
                if (!digitalRead(m_limitPin))
                {
                    m_magnetPos02 = currentPosition();
                    setCurrentPosition(0);
                    setMaxSpeed(speed);
                    setAcceleration(accel);
                    m_homingIndex = 1;

                    m_homingStateDouble = HomingStateDouble_e::SM_SECOND_PASS;
                }
                else if (Utils::MicroTimer(m_homingTimer, INTERVAL_FAST))
                {
                    moveTo(m_homingIndex);
                    m_homingIndex--;
                    run();
                }
            }
            break;
            case HomingStateDouble_e::SM_SECOND_PASS:
            {
                if (digitalRead(m_limitPin))
                {
                    m_magnetPos02 += currentPosition();
                    setCurrentPosition(0);
                    setMaxSpeed(speed);
                    setAcceleration(accel);
                    int32_t midPoint = m_magnetPos02 / 2;
                    (void) m_magnetPos01;
                    move(-midPoint);

                    m_homingStateDouble = HomingStateDouble_e::MOVE_MIDPOINT;
                }
                else if (Utils::MicroTimer(m_homingTimer, INTERVAL_SLOW))
                {
                    moveTo(m_homingIndex);
                    m_homingIndex++;
                    run();
                }
            }
            break;
            case HomingStateDouble_e::MOVE_MIDPOINT:
            {
                if (distanceToGo() != 0)
                {
                    run();
                }
                else
                {
                    stop();
                    setCurrentPosition(0);
                    setMaxSpeed(speed);
                    setAcceleration(accel);
                    moveTo(0);

                    m_homingStateDouble = HomingStateDouble_e::HOMING_COMPLETE;
                }
            }
            break;
            case HomingStateDouble_e::HOMING_COMPLETE:
            default:
                break;
        }
    }
    float AxialActuator::GetPosition()
    {
        return GetCurrentAngle();
    }
}// namespace Device