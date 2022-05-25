// ======================================
// Filename:    AxuialActuator.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_CONTROLLER_DEVICE_AXIAL_ACTUATOR_H
#define ROBOX_CONTROLLER_DEVICE_AXIAL_ACTUATOR_H

#include "Actuator.h"
#include "HallSensor.h"

namespace Device
{
    enum class HomingStyle_e : uint8_t;

    class AxialActuator : public Actuator
    {
    private:
        enum class HomingStateSingle_e : uint8_t
        {
            SETUP = 0,
            FIRST_PASS = 1,
            SECOND_PASS = 2,
            MOVING_OFFSET = 3,
            FOURTH_PASS = 4,
            ZERO_POS_MOVE = 5,
            HOMING_COMPLETE = 6,
        };
        enum class HomingStateDouble_e : uint8_t
        {
            SETUP = 0,
            FM_FIRST_PASS = 1,
            FM_SECOND_PASS = 2,
            MOVING_OFFSET = 3,
            SM_FIRST_PASS = 4,
            SM_SECOND_PASS = 5,
            MOVE_MIDPOINT = 6,
            HOMING_COMPLETE = 7,
        };

    public:
        AxialActuator(uint8_t stp, uint8_t dir, uint8_t enb, uint8_t limit, Microstepping_e microstepping,
                      float reduction, bool inverted, HomingStyle_e style);
        ~AxialActuator() override = default;
        /**
         * @brief Move actuator a set amount of degrees.
         * 
         * @param degrees Amount of degrees.
         */
        void MoveDegrees(float degrees);
        /**
         * @brief Move actuator to a set angle.
         * 
         * @param angle Angle to move to.
         */
        void MoveToAngle(float angle);
        /**
         * @brief Run actuator for a set amount of degrees.
         * 
         * @note This method blocks!
         * 
         * @param degrees Amount of degrees.
         */
        void RunDegrees(float degrees);
        /**
         * @brief Run actuator to a set angle.
         * 
         * @note This method blocks!
         * 
         * @param angle Angle to move to.
         */
        void RunToAngle(float angle);
        /**
         * @brief Get the current angle of the actuator.
         * 
         * @return float The current angle.
         */
        float GetCurrentAngle();
        /**
         * @brief Get the amount of degrees left until the target is reached.
         * 
         * @return float Amount of degrees until target is reached.
         */
        float GetDegreesToTarget();
        /**
         * @brief Run the homing sequence of this actuator. 
         * 
         * @note This method should be called as often as possible until the sequence is complete.
         * @see bool HomingComplete();
         * 
         * @param speed The speed of this actuator.
         * @param accel The acceleration of this actuator.
         */
        virtual void HomingSequence(float speed, float accel) override;
        /**
         * @brief Check is the homing sequence is completed.
         * 
         * @return true If the sequence is complete.
         * @return false If the sequence is NOT complete.
         */
        virtual bool HomingComplete() override;
        /**
         * @brief Resets homing states and other variables, should be called when a global reset occurs.
         * 
         */
        virtual void ResetHoming() override;
        /**
         * @brief Get the amount of steps per degree.
         * 
         * @return float Steps per degree.
         */
        float GetStepsPerDegree();

    protected:
        virtual float GetPosition() override;

    private:
        void HomeSingleMagnet(float speed, float accel);
        void HomeDoubleMagnet(float speed, float accel);

    private:
        float m_stepsPerDegree;

        uint8_t m_limitPin;
        Device::HallSensor m_switch;
        
        HomingStyle_e m_homingStyle;

        HomingStateDouble_e m_homingStateDouble;
        HomingStateSingle_e m_homingStateSingle;
        uint32_t m_homingTimer, m_homingIndex;
        int32_t m_magnetPos01, m_magnetPos02;
    };
    enum class HomingStyle_e : uint8_t
    {
        SINGLE_MAGNET = 0,
        DOUBLE_MAGNET = 1,
    };
}// namespace Device

#endif// ROBOX_CONTROLLER_DEVICE_AXIAL_ACTUATOR_H