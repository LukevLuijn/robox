// ======================================
// Filename:    RobotController.cpp
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        18-05-2022
// ======================================

#include "RobotController.h"
#include "Logger.h"

namespace Driver
{
    RobotController::RobotController() : m_directSend(false), m_robotState(RobotState_e::DISABLED)
    {
        std::fill(m_newTargetAngles.begin(), m_newTargetAngles.end(), 0);
        std::fill(m_gripTarget.begin(), m_gripTarget.end(), 0);
        std::fill(m_newConfig.begin(), m_newConfig.end(), 0);
    }
    void RobotController::StartConnection(const std::string& port, uint16_t baud,
                                          const std::function<void(const std::string&)>& callback)
    {
        m_driver.StartRobotConnection(port, baud, callback);

        m_driver.UpdateStaticData();
        m_driver.UpdateDynamicData();

        std::thread runTimeDataThread(&RobotDriver::UpdateRunTimeDataContinuously, &m_driver);
        runTimeDataThread.detach();
    }
    void RobotController::StopConnection()
    {
        m_driver.StopRobotConnection();
    }
    void RobotController::StartRobot()
    {
        if (m_robotState == RobotState_e::DISABLED)
        {
            m_robotState = RobotState_e::ENABLED;
            m_driver.StartRobot();
        }
    }
    void RobotController::StopRobot()
    {
        if (m_robotState == RobotState_e::ENABLED)
        {
            m_robotState = RobotState_e::DISABLED;
            m_driver.StopRobot();
        }
    }
    void RobotController::EStopRobot()
    {
        if (m_robotState == RobotState_e::ENABLED)
        {
            m_robotState = RobotState_e::DISABLED;
            m_driver.EmergencyStop();
        }
    }
    void RobotController::SetNewSpeed(float value)
    {
        m_newConfig[static_cast<size_t>(Config_e::SPEED)] = value;// / 100;
        DEBUG("New speed value", m_newConfig[static_cast<size_t>(Config_e::SPEED)]);
        SaveConfig(m_directSend);
    }
    void RobotController::SetNewAccel(float value)
    {
        m_newConfig[static_cast<size_t>(Config_e::ACCEL)] = value;// / 100;
        DEBUG("New acceleration value", m_newConfig[static_cast<size_t>(Config_e::ACCEL)]);
        SaveConfig(m_directSend);
    }
    void RobotController::SetNewPositionA0(float value)
    {
        DEBUG("New target height A0", value);
        m_newTargetAngles[static_cast<size_t>(AngleTargets_e::A0)] = value;
        RunRobot(m_directSend);
    }
    void RobotController::SetNewPositionA1(float value)
    {
        DEBUG("New target angle A1", value);
        m_newTargetAngles[static_cast<size_t>(AngleTargets_e::A1)] = value;
        RunRobot(m_directSend);
    }
    void RobotController::SetNewPositionA2(float value)
    {
        DEBUG("New target angle A2", value);
        m_newTargetAngles[static_cast<size_t>(AngleTargets_e::A2)] = value;
        RunRobot(m_directSend);
    }
    void RobotController::SetNewPositionA3(float value)
    {
        DEBUG("New target angle A3", value);
        m_newTargetAngles[static_cast<size_t>(AngleTargets_e::A3)] = value;
        RunRobot(m_directSend);
    }
    void RobotController::SetNewPositionA12(float value0, float value1)
    {
        std::vector<std::string> vars = {Utils::String::ToString(value0, 2), Utils::String::ToString(value1, 2)};
        DEBUG("New target angle A1 & A2", vars);

        m_newTargetAngles[static_cast<size_t>(AngleTargets_e::A1)] = value0;
        m_newTargetAngles[static_cast<size_t>(AngleTargets_e::A2)] = value1;
        RunRobot(m_directSend);
    }
    void RobotController::SetGripperValue01(uint8_t value)
    {
        DEBUG("New value gripper 00", (uint16_t) value);
        m_gripTarget[0] = value;
        RunRobot(m_directSend);
    }
    void RobotController::SetGripperValue02(uint8_t value)
    {
        DEBUG("New value gripper 01", (uint16_t) value);
        m_gripTarget[1] = value;
        RunRobot(m_directSend);
    }
    [[maybe_unused]] void RobotController::SetFrameOffset(float offset)
    {
        m_driver.SetFrameOffset(offset);
    }
    void RobotController::SetGripperOffset(float offset)
    {
        m_driver.SetGripperOffset(offset);
    }
    void RobotController::SetNewMove(const RobotMove& move, bool directSend)
    {
        m_gripTarget = move.m_gripper;
        m_newTargetAngles = move.m_target;
        m_newConfig = move.m_config;

        if (directSend)
        {
            SaveConfig();
            RunRobot();
        }
    }
    void RobotController::SaveConfig(bool send)
    {
        if (send)
        {
            std::array<float, 4> speeds = {
                    static_cast<float>(Driver::RobotDriver::m_dataManager.m_segment00.m_maxSpeed) *
                            (m_newConfig[static_cast<size_t>(Config_e::SPEED)] / 100),
                    static_cast<float>(Driver::RobotDriver::m_dataManager.m_segment01.m_maxSpeed) *
                            (m_newConfig[static_cast<size_t>(Config_e::SPEED)] / 100),
                    static_cast<float>(Driver::RobotDriver::m_dataManager.m_segment02.m_maxSpeed) *
                            (m_newConfig[static_cast<size_t>(Config_e::SPEED)] / 100),
                    static_cast<float>(Driver::RobotDriver::m_dataManager.m_segment03.m_maxSpeed) *
                            (m_newConfig[static_cast<size_t>(Config_e::SPEED)] / 100),
            };
            std::array<float, 4> accelerations = {
                    static_cast<float>(Driver::RobotDriver::m_dataManager.m_segment00.m_maxAccel) *
                            (m_newConfig[static_cast<size_t>(Config_e::ACCEL)] / 100),
                    static_cast<float>(Driver::RobotDriver::m_dataManager.m_segment01.m_maxAccel) *
                            (m_newConfig[static_cast<size_t>(Config_e::ACCEL)] / 100),
                    static_cast<float>(Driver::RobotDriver::m_dataManager.m_segment02.m_maxAccel) *
                            (m_newConfig[static_cast<size_t>(Config_e::ACCEL)] / 100),
                    static_cast<float>(Driver::RobotDriver::m_dataManager.m_segment03.m_maxAccel) *
                            (m_newConfig[static_cast<size_t>(Config_e::ACCEL)] / 100),
            };
            m_driver.SetSpeedAccel(speeds, accelerations);
        }
    }
    void RobotController::RunRobot(bool send)
    {
        if (send)
        {
            SaveConfig();

            m_driver.MoveRobotForward(m_newTargetAngles);
            m_driver.MoveRobotGripper(m_gripTarget[0], m_gripTarget[1]);
        }
    }
    void RobotController::SetDirectSend(bool directSend)
    {
        m_directSend = directSend;
    }
    bool RobotController::GetDirectSend() const
    {
        return m_directSend;
    }
    bool RobotController::IsConnected() const
    {
        return m_driver.IsConnected();
    }
}// namespace Driver