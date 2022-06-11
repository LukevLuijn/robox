// ======================================
// Filename:    RobotController.h
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        18-05-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_ROBOT_CONTROLLER_H
#define ROBOX_CONTROL_UI_ROBOT_CONTROLLER_H

#include "RobotDriver.h"
#include <array>

namespace Driver
{
    enum class RobotState_e : uint8_t;

    struct RobotMove {
        std::array<float, 4> m_target;
        std::array<uint8_t, 2> m_gripper;
        std::array<float, 2> m_config;
    };

    class RobotController
    {
    private:
        RobotController();

    public:
        [[maybe_unused]] RobotController(const RobotController* other) = delete;
        RobotController& operator=(const RobotController& other) = delete;
        virtual ~RobotController() = default;

        /**
         * @brief Get the instance of the robotController.
         * 
         * @return RobotController& 
         */
        static RobotController& GetInstance()
        {
            static RobotController instance;
            return instance;
        }

    public:
        /**
         * @brief Start the serial connection with the device.
         * 
         * @param port Port name e.g. /dev/ttyACM0
         * @param baud BAUD rate e.g. 9600
         * @param callback Callback method for the read messages.
         */
        void StartConnection(
                const std::string& port, uint16_t baud,
                const std::function<void(const std::string&)>& callback = [](const std::string& message) {
                    INFO("Response:", message);
                });
        /**
         * @brief Stop the serial connection with the device.
         * 
         */
        void StopConnection();
        /**
         * @brief Start the robot.
         * 
         */
        void StartRobot();
        /**
         * @brief Stop the robot.
         * 
         */
        void StopRobot();
        /**
         * @brief Perform a emergency stop
         * 
         */
        void EStopRobot();
        /**
         * @brief Pause the robot.
         *
         * @param time Pause time in milliseconds.
         */
        void PauseRobot(uint32_t time);
        /**
         * @brief Set new speed value for the device.
         * 
         * @param value The new speed value. 
         */
        void SetNewSpeed(float value);
        /**
         * @brief Set new acceleration value for the device.
         * 
         * @param value The new acceleration value.
         */
        void SetNewAccel(float value);
        /**
         * @brief Set a new position for axis 0.
         * 
         * @param value New position in millimeters.
         */
        void SetNewPositionA0(float value);
        /**
         * @brief Set a new position for axis 1.
         * 
         * @param value New position in degrees.
         */
        void SetNewPositionA1(float value);
        /**
         * @brief Set a new position for axis 2.
         * 
         * @param value New position in degrees.
         */
        void SetNewPositionA2(float value);
        /**
         * @brief Set a new position for axis 3.
         * 
         * @param value New position in degrees.
         */
        void SetNewPositionA3(float value);
        /**
         * @brief Set a new position for both axis 1 and 2.
         * 
         * @param value0 New position axis 1 in degrees.
         * @param value1 New position axis 2 in degrees.
         */
        void SetNewPositionA12(float value0, float value1);
        /**
         * @brief Set the value for gripper signal pin 1.
         * 
         * @param value new PWM value for gripper 1 [0..255].
         */
        void SetGripperValue01(uint8_t value);
        /**
         * @brief Set the value for gripper signal pin 2.
         * 
         * @param value new PWM value for gripper 1 [0..255].
         */
        void SetGripperValue02(uint8_t value);
        /**
         * @brief Set the frame offset.
         * 
         * @param offset 
         */
        [[maybe_unused]] void SetFrameOffset(float offset);
        /**
         * @brief Set the gripper offset.
         * 
         * @param offset 
         */
        [[maybe_unused]] void SetGripperOffset(float offset);
        /**
         * @brief Add a new move to the movement queue.
         * 
         * @param move New move.
         * @param directSend True if it should be send directly, false if not.
         */
        [[maybe_unused]] void SetNewMove(const RobotMove& move, bool directSend);
        /**
         * @brief Convert and save the buffered config values.
         * 
         * @param send True if the converted data should be send directly, false if not.
         */
        void SaveConfig(bool send = true);
        /**
         * @brief Convert and save the buffered movement values.
         * 
         * @param send True if the converted data should be send directly, false if not.
         */
        void RunRobot(bool send = true);
        /**
         * @brief Set the 'directSend' flag for the controller.
         * 
         * @param directSend True if all new messages should be send directly, false if not.
         */
        void SetDirectSend(bool directSend);
        /**
         * @brief Get the directSend value
         * 
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool GetDirectSend() const;
        /**
         * @brief Check if the driver is currently connected to the robot.
         * 
         * @return true If the driver is currently connected.
         * @return false If the dirver is currently NOT connected.
         */
        [[nodiscard]] bool IsConnected() const;

    private:
        RobotDriver m_driver;
        bool m_directSend;
        std::array<float, 4> m_newTargetAngles{};
        std::array<uint8_t, 2> m_gripTarget{};
        std::array<float, 2> m_newConfig{};

        std::vector<RobotMove> m_moves;
        RobotState_e m_robotState;
    };

    enum class AngleTargets_e : uint8_t
    {
        A0 = 0,
        A1 = 1,
        A2 = 2,
        A3 = 3,
    };
    enum class Config_e : uint8_t
    {
        SPEED = 0,
        ACCEL = 1,
    };
    enum class RobotState_e : uint8_t
    {
        ENABLED,
        DISABLED,
    };
}// namespace Driver

#endif//ROBOX_CONTROL_UI_ROBOT_CONTROLLER_H
