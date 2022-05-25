// ======================================
// Filename:    Robot.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_CONTROLLER_BASE_ROBOT_H
#define ROBOX_CONTROLLER_BASE_ROBOT_H

#include "config/config.h"
#include "device/AxialActuator.h"
#include "device/LinearActuator.h"

namespace Base
{
    enum class Joint_e : uint8_t;
    /**
     * @brief Simple 3D point.
     * 
     */
    struct Point {
        float X;// Position x in mm.
        float Y;// Position y in mm.
        float Z;// Position z in mm.
    };
    /**
     * @brief Position of gripper, including gripper angle.
     * 
     * @note Used for inverse kinametics.
     * 
     */
    struct Position {
        float X;// Position x in mm.
        float Y;// Position y in mm.
        float Z;// Position z in mm.
        float G;// Angle in degrees.
    };
    /**
     * @brief Angles for the joints, including height.
     * 
     * @note Used for forward kinametics. 
     * 
     */
    struct Angles {
        float Z0; // height in mm.
        float A1; // angle in degrees.
        float A2; // angle in degrees.
        float A3; // angle in degrees.
    };
    class Robot
    {
    public:
        /**
         * @brief Get Robot instance.
         * 
         * @return Robot& 
         */
        static Robot& GetInstance()
        {
            static Robot instance;
            return instance;
        }
        /**
         * @brief Start robot instance.
         * 
         */
        void Start();
        /**
         * @brief Run robot instance.
         * 
         * @note Should be called as often as possible.
         * 
         */
        void Run();
        /**
         * @brief Set new target position for robot.
         * 
         * @param target New target position.
         */
        void SetTarget(const Position& target);
        /**
         * @brief Set new target angles for robot.
         * 
         * @param target New target angles.
         */
        void SetTarget(const Angles& target);
        /**
         * @brief Disable all robot joints.
         * 
         * @note After disabling the robot should be homed!
         * 
         */
        void DisableRobot();
        /**
         * @brief Disable single robot joint.
         * 
         * @note After disabling the robot should be homed!
         * 
         * @param joint Joint index to disable.
         */
        void DisableJoint(Joint_e joint);
        /**
         * @brief Enable all robot joints.
         * 
         */
        void EnableRobot();
        /**
         * @brief Enable single robot joint.
         * 
         * @param joint Joint index to enable.
         */
        void EnableJoint(Joint_e joint);
        /**
         * @brief Execute homing sequence.
         * 
         * @note While the homing sequence is incomplete this method should be called as often as possible.
         * @see bool IsHomingComplete();
         * 
         */
        void HomingSequence();
        /**
         * @brief Resets homing states and other variables, should be called when a global reset occurs.
         * 
         */
        void ResetHoming();
        /**
         * @brief Set PWM value for the first gripper signal pin.
         * 
         * @param signal PWM signal.
         */
        void SetGripperPWMSignal00(uint8_t signal);
        /**
         * @brief Set PWM value for the second gripper signal pin.
         * 
         * @param signal PWM signal.
         */
        void SetGripperPWMSignal01(uint8_t signal);
        /**
         * @brief Get PWM value for the first gripper signal pin.
         * 
         * @return uint8_t PWM value.
         */
        uint8_t GetGripperPWMSignal00() const;
        /**
         * @brief Get PWM value for the second gripper signal pin.
         * 
         * @return uint8_t PWM value.
         */
        uint8_t GetGripperPWMSignal01() const;
        /**
         * @brief Get the current position of the gripper.
         * 
         * @return Point Gripper coordinates.
         */
        Point GetPosition();
        /**
         * @brief Get the position of a joint.
         * 
         * @param joint Index of the joint.
         * @return float Current position of the joint, based on the joint this could be in millimeters or degrees.
         */
        float GetJointPosition(Joint_e joint);
        /**
         * @brief Check if a joint is currently running.
         * 
         * @param joint Index of the joint.
         * @return true If the joint is currently is running.
         * @return false if the joint is currently NOT running.
         */
        bool IsRunning(Joint_e joint);
        /**
         * @brief Check if the Robot is currently running.
         * 
         * @return true If the Robot is currently running.
         * @return false If the Robot is currently NOT running.
         */
        bool IsRunning();
        /**
         * @brief Check if the last set target is reached.
         * 
         * @return true If last set target is reached.
         * @return false If last set target is NOT reached.
         */
        bool IsMoveComplete();
        /**
         * @brief Check if the homing sequence is completed.
         * 
         * @return true If the homing sequence is completed.
         * @return false If the homing sequence is NOT completed.
         */
        bool IsHomingComplete();
        /**
         * @brief Set max speed for all Robot joints.
         * 
         * @param speed 
         */
        void SetSpeed(float speed);
        /**
         * @brief Set acceleration for all Robot joints.
         * 
         * @param accel 
         */
        void SetAccel(float accel);
        /**
         * @brief Set the max speed for a single Robot joint.
         * 
         * @param joint Index of the joint.
         * @param speed 
         */
        void SetSpeed(Joint_e joint, float speed);
        /**
         * @brief Set the max acceleration of a single Robot joint.
         * 
         * @param joint Index of the joint.
         * @param accel 
         */
        void SetAccel(Joint_e joint, float accel);
        /**
         * @brief Get the max speed of a single Robot joint.
         * 
         * @param joint Index of the joint.
         * @return float Max speed of the requested joint.
         */
        float GetSpeed(Joint_e joint);
        /**
         * @brief Get the accleration of a single Robot joint.
         * 
         * @param joint Index of the joint.
         * @return float Acceleration of the requested joint.
         */
        float GetAccel(Joint_e joint);
        /**
         * @brief Get the microstepping configuration of a single joint.
         * 
         * @param joint Index of the joint.
         * @return Device::Microstepping_e 
         */
        Device::Microstepping_e GetMicrostepping(Joint_e joint);
        /**
         * @brief Get the steps per type value for a single joint.
         * 
         * @note For linear actuators this value is steps per millimeter, for axial actuators this value is steps per degree.
         * 
         * @param joint Index of the joint.
         * @return float Steps per type value.
         */
        float GetStepsPerType(Joint_e joint);

    private:
        Robot();

    public:
        Robot(const Robot&) = delete;
        void operator=(const Robot&) = delete;

    private:
        Angles Translate(Position target);
        Position Translate(Angles target);

        bool CheckBounds(const Angles& target);

    private:
        Device::LinearActuator Z0;
        Device::AxialActuator A1, A2, A3;

        uint8_t m_valueGripper00, m_valueGripper01;
    };

    /**
     * @brief Robot joint indices.
     * 
     */
    enum class Joint_e : uint8_t
    {
        Z0 = 0,
        A1 = 1,
        A2 = 2,
        A3 = 3,
    };
}// namespace Base

#endif// ROBOX_CONTROLLER_BASE_ROBOT_H
