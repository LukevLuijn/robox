//
// Created by luke on 14-05-22.
//

#include "RobotDriver.h"
#include "../include/logger/Logger.h"

#include "global_config.h"

namespace Driver
{
    /*static*/ DataManager RobotDriver::m_dataManager;
    namespace
    {
        void StartSerial(RobotDriver* driver, const std::string& port, uint16_t baud)
        {
            auto callback = std::bind(&RobotDriver::ReadMessage, driver, std::placeholders::_1, std::placeholders::_2);
            driver->Start(callback, port, baud);
        }
    }// namespace

    RobotDriver::RobotDriver() : m_updateRunTimeData(false)
    {
    }
    RobotDriver::~RobotDriver()
    {
        Stop();
    }
    void RobotDriver::StartRobotConnection(const std::string& port, uint16_t baud,
                                           const std::function<void(const std::string&)>& callback)
    {
        StartSerial(this, port, baud);
        m_responseCallback = callback;
    }
    void RobotDriver::StopRobotConnection()
    {
        m_updateRunTimeData = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        Stop();
    }
    void RobotDriver::MoveRobotInverse(const MoveArray& position)
    {
        const std::string header = CreateMessageHeader(MessageType_e::MOVE, (uint8_t) MoveType_e::SET_INVERSE);
        const std::string body = "[" + CreateParamString(position) + "]";
        Write(header + body);
    }
    void RobotDriver::MoveRobotForward(const MoveArray& angles)
    {
        const std::string header = CreateMessageHeader(MessageType_e::MOVE, (uint8_t) MoveType_e::SET_FORWARD);
        const std::string body = "[" + CreateParamString(angles) + "]";
        Write(header + body);
    }
    void RobotDriver::MoveRobotGripper(uint8_t gripper01, uint8_t gripper02)
    {
        const std::string header = CreateMessageHeader(MessageType_e::MOVE, (uint8_t) MoveType_e::SET_GRIPPER);
        const std::string body = "[" + std::to_string(gripper01) + "," + std::to_string(gripper02) + "]";
        Write(header + body);
    }
    void RobotDriver::StartRobot()
    {
        const std::string header = CreateMessageHeader(MessageType_e::START, 0);
        Write(header + "[]");
    }
    void RobotDriver::PauseRobot(uint32_t time)
    {
        const std::string header = CreateMessageHeader(MessageType_e::STOP, (uint8_t) StopType_e::SET_PAUSE);
        const std::string body = "[" + std::to_string(time) + "]";
        Write(header + body);
    }
    void RobotDriver::StopRobot()
    {
        const std::string header = CreateMessageHeader(MessageType_e::STOP, (uint8_t) StopType_e::STOP_ROBOT);
        Write(header + "[]");
    }
    void RobotDriver::EmergencyStop()
    {
        const std::string header = CreateMessageHeader(MessageType_e::STOP, (uint8_t) StopType_e::ESTOP_ROBOT);
        Write(header + "[]");
    }
    void RobotDriver::SetSpeedAccel(float speed, float accel)
    {
        const std::string header = CreateMessageHeader(MessageType_e::DATA, (uint8_t) DataType_e::SET_SPEED_ACCEL);
        const std::string body = "[" + std::to_string(speed) + "," + std::to_string(accel) + "]";
        Write(header + body);
    }
    void RobotDriver::SetSpeedAccel(const SpeedArray& speeds, const AccelArray& accelerations)
    {
        const std::string header = CreateMessageHeader(MessageType_e::DATA, (uint8_t) DataType_e::SET_SPEED_ACCEL);
        const std::string body = "[" + CreateParamString(speeds) + "," + CreateParamString(accelerations) + "]";
        Write(header + body);
    }
    void RobotDriver::SetVerbosity(const VerbArray& verbs)
    {
        const std::string header = CreateMessageHeader(MessageType_e::DATA, (uint8_t) DataType_e::SET_VERBOSITY);
        std::string body = "[";

        std::array<std::string, 4> paramList;

        paramList[(uint8_t) VerbosityType_e::ERROR] = (verbs[(uint8_t) VerbosityType_e::ERROR]) ? "true" : "false";
        paramList[(uint8_t) VerbosityType_e::WARNING] = (verbs[(uint8_t) VerbosityType_e::WARNING]) ? "true" : "false";
        paramList[(uint8_t) VerbosityType_e::INFO] = (verbs[(uint8_t) VerbosityType_e::INFO]) ? "true" : "false";
        paramList[(uint8_t) VerbosityType_e::ERROR] = (verbs[(uint8_t) VerbosityType_e::ERROR]) ? "true" : "false";

        for (std::size_t i = 0; i < paramList.size(); ++i)
        {
            body.append(paramList[i]).append((i != paramList.size() - 1) ? "," : "");
        }
        body.append("]");
        Write(header + body);
    }
    void RobotDriver::SetFrameOffset(float frameOffset)
    {
        const std::string header = CreateMessageHeader(MessageType_e::DATA, (uint8_t) DataType_e::SET_FRAME_OFFSET);
        Write(header + "[" + std::to_string(frameOffset) + "]");
    }
    void RobotDriver::SetGripperOffset(float gripperOffset)
    {
        const std::string header = CreateMessageHeader(MessageType_e::DATA, (uint8_t) DataType_e::SET_GRIP_OFFSET);
        Write(header + "[" + std::to_string(gripperOffset) + "]");
    }
    void RobotDriver::SetIdlePositions(const IdlePositions& idlePositions)
    {
        const std::string header = CreateMessageHeader(MessageType_e::DATA, (uint8_t) DataType_e::SET_IDLE_POSITIONS);
        const std::string body = "[" + CreateParamString(idlePositions) + "]";
        Write(header + body);
    }
    void RobotDriver::UpdateDynamicData()
    {
        const std::string header = CreateMessageHeader(MessageType_e::DATA, (uint8_t) DataType_e::GET_DYNAMIC);
        Write(header + "[]");
    }
    void RobotDriver::UpdateStaticData()
    {
        const std::string header = CreateMessageHeader(MessageType_e::DATA, (uint8_t) DataType_e::GET_STATIC);
        Write(header + "[]");
    }
    void RobotDriver::UpdateRunTimeData()
    {
        const std::string header = CreateMessageHeader(MessageType_e::DATA, (uint8_t) DataType_e::GET_RUNTIME);
        Write(header + "[]");
    }
    void RobotDriver::UpdateRunTimeDataContinuously()
    {
        m_updateRunTimeData = true;
        uint8_t connectionCounter = 0;
        INFO("Started retrieving runtime data");
        while (m_updateRunTimeData)
        {
            if (!IsConnected())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                ++connectionCounter;

                if (connectionCounter >= 5)
                {
                    break;
                }
            }
            UpdateRunTimeData();
            std::this_thread::sleep_for(std::chrono::milliseconds(RUN_TIME_DATA_UPDATE_DELAY_MS));
        }
        INFO("Stopped retrieving runtime data");
    }

    std::string RobotDriver::CreateParamString(const MoveArray& array)
    {
        std::string paramString;

        for (auto it = array.begin(); it != array.end(); ++it)
        {
            paramString += Utils::String::ToString(*it, 2);
            paramString += (std::next(it) != array.end()) ? "," : "";
        }
        return paramString;
    }
    std::string RobotDriver::CreateMessageHeader(MessageType_e type, uint8_t index)
    {
        std::string header = static_cast<char>(type) + std::to_string(index);
        return header;
    }
    void RobotDriver::ReadMessage(const char* data, unsigned int len)
    {
        const std::string message = std::string(data, len);

        if (message[0] == RESPONSE_START_CHAR && *(message.end() - 1) == RESPONSE_STOP_CHAR)
        {
            std::string content = message.substr(1, message.length() - 2);
            m_dataManager.ParseMessage(message.substr(1, message.length() - 2));
            m_responseCallback(content);
        }
        else
        {
            std::vector<std::string> logs = Utils::String::Divide(message, LOG_START_CHAR, LOG_STOP_CHAR);

            for (const std::string& log : logs)
            {
                Logger::Logger::GetInstance().LogExternal(log);
            }
        }
    }

}// namespace Driver