// ======================================
// Filename:    Actuator.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_CONTROLLER_DEVICE_ACTUATOR_H
#define ROBOX_CONTROLLER_DEVICE_ACTUATOR_H

#include <AccelStepper.h>
#include <Arduino.h>

namespace Device
{
    enum class Microstepping_e : uint8_t;

    class Actuator : public AccelStepper
    {
    public:
        Actuator(uint8_t stp, uint8_t dir, uint8_t enb, Microstepping_e microstepping, float reduction, bool inverted);
        virtual ~Actuator() = default;
        /**
         * @brief Start the actuator instance.
         * 
         */
        void Start();
        /**
         * @brief Run the actuator instance.
         * 
         * @note This method should be called as often as possible.
         * 
         */
        void Run();
        /**
         * @brief Get the microstepping configuration of this actuator.
         * 
         * @return Microstepping_e 
         */
        Microstepping_e GetMicrostepping() const;
        /**
         * @brief Get the steps per revolution for this actuator.
         * 
         * @note The revolution is an full revolution of the actuator output, NOT the motor.
         * 
         * @return float The steps per revolution.
         */
        float GetStepsPerRevolution() const;
        /**
         * @brief Set the acceleration of this actuator.
         * 
         * @param acceleration 
         */
        void SetAcceleration(float acceleration);
        /**
         * @brief Get the current acceleration of this actuator.
         * 
         * @return float The current acceleration.
         */
        float GetAcceleration();
        /**
         * @brief Pure virtual, can not be called.
         * 
         */
        virtual void HomingSequence(float homingSpeed, float homingAccel, float speed, float accel ) = 0;
        /**
         * @brief Pure virtual, can not be called.
         * 
         */
        virtual bool HomingComplete() = 0;
        /**
         * @brief Pure virtual, can not be called.
         * 
         */
        virtual void ResetHoming() = 0;

    protected:
        /**
         * @brief Pure virtual, can not be called.
         * 
         */
        virtual float GetPosition() = 0;

    protected:
        Microstepping_e m_microstepping;
        float m_stepsPerRevolution;
        float m_acceleration;
    };
    /**
     * @brief Possible microstepping configurations.
     * 
     * @note Based on the DM332T stepper driver.
     *       https://www.omc-stepperonline.com/digital-stepper-drive-1-0-3-2a-18-30vdc-for-nema-17-23-stepper-motor-dm332t
     * 
     */
    enum class Microstepping_e : uint8_t
    {
        MS400 = 2,
        MS800 = 4,
        MS1600 = 8,
        MS3200 = 16,
        MS6400 = 32,
        MS12800 = 64,
    };
}// namespace Device

#endif//ROBOX_CONTROLLER_DEVICE_ACTUATOR_H