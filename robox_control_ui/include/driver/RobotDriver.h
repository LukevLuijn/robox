//
// Created by luke on 14-05-22.
//

#ifndef ROBOX_UI_LOW_LEVEL_DRIVER_H
#define ROBOX_UI_LOW_LEVEL_DRIVER_H

#include <string>
#include <utility>

#include "../logger/Logger.h"
#include "DataManager.h"
#include "SerialDriver.h"
#include "msg_protocol.h"

namespace Driver
{
    class RobotDriver : public SerialDriver
    {
    private:
        typedef std::array<float, 4> MoveArray;
        typedef std::array<float, 4> SpeedArray;
        typedef std::array<float, 4> AccelArray;
        typedef std::array<bool, 4> VerbArray;
        typedef std::array<float, 4> IdlePositions;

    public:
        explicit RobotDriver();
        ~RobotDriver() override;

        /**
         * @brief Start the serial connection with the device.
         * 
         * @param port Port name e.g. /dev/ttyACM0
         * @param baud BAUD rate e.g. 9600
         * @param callback Callback method for the read messages.
         */
        void StartRobotConnection(
                const std::string& port, uint16_t baud,
                const std::function<void(const std::string&)>& callback = [](const std::string& message) {
                    INFO("Response:", message);
                });
        /**
         * @brief Stop the serial connection with the device.
         * 
         */
        void StopRobotConnection();
        /**
         * @brief Send cartesian positions over serial.
         * 
         * @param position Positions in cartesian space [x,y,z,g].
         */
        void MoveRobotInverse(const MoveArray& position);
        /**
         * @brief Send angle positions over serial.
         * 
         * @param angles Angles for the joints in degrees.
         */
        void MoveRobotForward(const MoveArray& angles);
        /**
         * @brief Send PWM values over serial.
         * 
         * @param gripper01 gripper 1 value [0..255]
         * @param gripper02 gripper 2 value [0..255]
         */
        void MoveRobotGripper(uint8_t gripper01, uint8_t gripper02);
        /**
         * @brief Send start message to robot.
         * 
         */
        void StartRobot();
        /**
         * @brief Send pause message to robot.
         * 
         * @param time Pause duration in milliseconds.
         */
        void PauseRobot(uint32_t time);
        /**
         * @brief Send stop message to the robot.
         * 
         */
        void StopRobot();
        /**
         * @brief Send emergency stop message to the robot.
         * 
         */
        void EmergencyStop();
        /**
         * @brief Set the speed and accel values for all the joints.
         * 
         * @param speed 
         * @param accel 
         */
        void SetSpeedAccel(float speed, float accel);
        /**
         * @brief Set the speed and accel values for each joint separately.
         * 
         * @param speeds 
         * @param accelerations 
         */
        void SetSpeedAccel(const SpeedArray& speeds, const AccelArray& accelerations);
        /**
         * @brief Set the verbosity values for the robot.
         * 
         * @param verbs True or false for each log message type.
         */
        void SetVerbosity(const VerbArray& verbs);
        /**
         * @brief Set the frame offset value.
         * 
         * @param frameOffset 
         */
        void SetFrameOffset(float frameOffset);
        /**
         * @brief Set the gripper offset value.
         * 
         * @param gripperOffset 
         */
        void SetGripperOffset(float gripperOffset);
        /**
         * @brief Set the idle positions for the robot.
         * 
         * @param idlePositions 
         */
        void SetIdlePositions(const IdlePositions& idlePositions);
        /**
         * @brief Send out a request for the dynamic data.
         * 
         */
        void UpdateDynamicData();
        /**
         * @brief Send out a request for the static data.
         * 
         */
        void UpdateStaticData();
        /**
         * @brief Send out a request for the run time data.
         * 
         */
        void UpdateRunTimeData();
        /**
         * @brief Should not be run in the main thread, sends out run time data requests continuously.
         * 
         */
        void UpdateRunTimeDataContinuously();
        /**
         * @brief Callback for the SerialDriver, parses incoming messages.
         * 
         * @param data the incoming data.
         * @param len  length of the incoming data.
         */
        void ReadMessage(const char* data, unsigned int len);

    private:
        static std::string CreateParamString(const MoveArray& array);
        static std::string CreateMessageHeader(MessageType_e type, uint8_t index);

    public:
        static DataManager m_dataManager;

    private:
        std::function<void(const std::string&)> m_responseCallback;
        bool m_updateRunTimeData;
    };
}// namespace Driver

#endif//ROBOX_UI_LOW_LEVEL_DRIVER_H
