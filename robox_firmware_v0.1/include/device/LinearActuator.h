// ======================================
// Filename:    LinearActuator.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_CONTROLLER_DEVICE_LINEAR_ACTUATOR_H
#define ROBOX_CONTROLLER_DEVICE_LINEAR_ACTUATOR_H

#include "Actuator.h"
#include "HallSensor.h"

namespace Device
{
    class LinearActuator : public Actuator
    {
    private:
        enum class HomingState_e : uint8_t
        {
            SETUP = 0,
            FIRST_PASS = 1,
            SECOND_PASS = 3,
            HOMING_COMPLETE = 5,
        };

    public:
        LinearActuator(uint8_t stp, uint8_t dir, uint8_t enb, uint8_t limit, Microstepping_e microstepping,
                       float reduction, bool inverted);
        ~LinearActuator() override = default;
        /**
         * @brief Move the actuator for a set distance.
         * 
         * @param distance Distance in millimeters.
         */
        void MoveDistance(float distance);
        /**
         * @brief Move the actuator to a set height.
         * 
         * @param height Height in millimeters.
         */
        void MoveToHeight(float height);
        /**
         * @brief Run the actuator for a set distance.
         * 
         * @note This method blocks!
         * 
         * @param distance Distance in millimeters.
         */
        void RunDistance(float distance);
        /**
         * @brief Run the actuator to a set height.
         * 
         * @note This method blocks!
         * 
         * @param height Height in millimeters.
         */
        void RunToHeight(float height);
        /**
         * @brief Get the current height of this actuator.
         * 
         * @return float Current height in millimeters.
         */
        float GetCurrentHeight();
        /**
         * @brief Get the remaining distance to last set target.
         * 
         * @return float Remaining distance in millimeters.
         */
        float GetDistanceToTarget();
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
         * @brief Get the steps per millimeter for this actuator.
         * 
         * @return float 
         */
        float GetStepsPerMillimeter();

    protected:
        virtual float GetPosition() override;
        float GetHeightForPosition(float position);

    private:
        float m_stepsPerMillimeter;
        
        uint8_t m_limitPin;
        Device::HallSensor m_switch;

        HomingState_e m_homingState;
        uint32_t m_homingTimer, m_homingIndex;
    };


}// namespace Device

#endif// ROBOX_CONTROLLER_DEVICE_LINEAR_ACTUATOR_H