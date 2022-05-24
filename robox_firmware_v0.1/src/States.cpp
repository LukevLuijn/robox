// ======================================
// Filename:    States.cpp
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#include "base/States.h"
#include "base/Robot.h"
#include "communication/Reader.h"

#include "utils/Logger.h"

namespace Base
{
    // ===================================================
    // STATE START-----------------------------STATE START
    // ===================================================
    bool StateStart::HandleEvent(const StateMachine::Event& event, StateMachine::Context& context)
    {
        bool success = false;

        if (event.GetMessageType() == MessageType_e::START)
        {
            auto state = std::make_shared<StateHome>(m_machine);
            context.SetCurrentState(state);
            success = true;
        }
        return success;
    }
    void StateStart::EntryAction()
    {
        Communication::Message message = Communication::Reader::GetInstance().GetMessage();
        Base::Robot::GetInstance().DisableRobot();
    }
    void StateStart::DoActivity()
    {
        m_activityComplete = true;
    }
    void StateStart::ExitAction()
    {
        Base::Robot::GetInstance().EnableRobot();
        Base::Robot::GetInstance().ResetHoming();
    }
    // ===================================================
    // STATE HOME-------------------------------STATE HOME
    // ===================================================
    bool StateHome::HandleEvent(const StateMachine::Event& event, StateMachine::Context& context)
    {
        bool success = false;

        switch (event.GetMessageType())
        {
            case MessageType_e::STOP:
            {
                auto state = std::make_shared<StateStop>(m_machine);
                context.SetCurrentState(state);
                success = true;
            }
            break;
            case MessageType_e::MOVE:
            {
                auto state = std::make_shared<StateMove>(m_machine);
                context.SetCurrentState(state);
                success = true;
            }
            break;
            case MessageType_e::RESET:
            {
                auto state = std::make_shared<StateStart>(m_machine);
                context.SetCurrentState(state);
                success = true;
            }
            break;
            case MessageType_e::DATA:
            case MessageType_e::START:
            case MessageType_e::UNDEFINED:
            default:
                break;
        }
        return success;
    }
    void StateHome::EntryAction()
    {
        Communication::Message message = Communication::Reader::GetInstance().GetMessage();
    }
    void StateHome::DoActivity()
    {
        Base::Robot::GetInstance().HomingSequence();

        if (Base::Robot::GetInstance().IsHomingComplete())
        {
            if (!m_activityComplete)
            {
                char type = static_cast<char>(MessageType_e::DATA);
                uint8_t index = static_cast<uint8_t>(DataType_e::HOMING_COMPLETE);
                String message = type + String(index) + "[]";

                Communication::Reader::GetInstance().GetInstance().WriteMessage(message);
            }
            m_activityComplete = true;
        }
    }
    void StateHome::ExitAction()
    {
    }
    // ===================================================
    // STATE MOVE-------------------------------STATE MOVE
    // ===================================================
    bool StateMove::HandleEvent(const StateMachine::Event& event, StateMachine::Context& context)
    {
        bool success = false;

        switch (event.GetMessageType())
        {
            case MessageType_e::MOVE:
            {
                auto state = std::make_shared<StateMove>(m_machine);
                context.SetCurrentState(state);
                success = true;
            }
            break;
            case MessageType_e::STOP:
            {
                auto state = std::make_shared<StateStop>(m_machine);
                context.SetCurrentState(state);
                success = true;
            }
            break;
            case MessageType_e::RESET:
            {
                auto state = std::make_shared<StateStart>(m_machine);
                context.SetCurrentState(state);
                success = true;
            }
            break;
            case MessageType_e::DATA:
            case MessageType_e::START:
            case MessageType_e::UNDEFINED:
            default:
                break;
        }

        return success;
    }
    void StateMove::EntryAction()
    {
        Communication::Message message = Communication::Reader::GetInstance().GetMessage();

        Communication::Buffer buffer;
        uint8_t bufferSize = 0;
        message.GetBody().GetBuffer(buffer, bufferSize);

        switch ((MoveType_e) message.m_header.m_index)
        {
            case MoveType_e::SET_FORWARD:
            {
                if (message.m_body.m_bufferSize == 4)
                {
                    Base::Angles position{buffer[0].toFloat(),
                                          buffer[1].toFloat(),
                                          buffer[2].toFloat(),
                                          buffer[3].toFloat()};
                    Base::Robot::GetInstance().SetTarget(position);
                }
                else
                {
                     INFO("To few arguments for move request", String((uint16_t)bufferSize));

                }
            }
            break;
            case MoveType_e::SET_INVERSE:
            {
                if (message.m_body.m_bufferSize == 4)
                {
                    Base::Position position{buffer[0].toFloat(),
                                            buffer[1].toFloat(),
                                            buffer[2].toFloat(),
                                            buffer[3].toFloat()};
                    Base::Robot::GetInstance().SetTarget(position);
                }
                else
                {
                    INFO("To few arguments for move request", String((uint16_t)bufferSize));

                }
            }
            break;
            case MoveType_e::SET_GRIPPER:
            {
                if (message.m_body.m_bufferSize == 2)
                {
                    Robot::GetInstance().SetGripperPWMSignal00((uint8_t) message.m_body.m_variables[0].toInt());
                    Robot::GetInstance().SetGripperPWMSignal01((uint8_t) message.m_body.m_variables[1].toInt());
                }
                else
                {
                    INFO("To few arguments for move request", String((uint16_t)bufferSize));
                }
            }
            default:
                DEBUG("MoveType_e undefined", String(message.m_header.m_index));
                break;
        }
    }
    void StateMove::DoActivity()
    {
        if (Base::Robot::GetInstance().IsMoveComplete())
        {
            m_activityComplete = true;
        }
    }
    void StateMove::ExitAction()
    {
    }
    // ===================================================
    // STATE SHUTDOWN-----------------------STATE SHUTDOWN
    // ===================================================
    bool StateStop::HandleEvent(const StateMachine::Event& event, StateMachine::Context& context)
    {
        bool success = false;

        switch (event.GetMessageType())
        {
            case MessageType_e::STOP:
            {
                auto state = std::make_shared<StateStop>(m_machine);
                context.SetCurrentState(state);
                success = true;
            }
            break;
            case MessageType_e::MOVE:
            {
                auto state = std::make_shared<StateMove>(m_machine);
                context.SetCurrentState(state);
                success = true;
            }
            break;
            case MessageType_e::RESET:
            {
                auto state = std::make_shared<StateStart>(m_machine);
                context.SetCurrentState(state);
                success = true;
            }
            break;
            case MessageType_e::DATA:
            case MessageType_e::START:
            case MessageType_e::UNDEFINED:
            default:
                break;
        }

        return success;
    }
    void StateStop::EntryAction()
    {
        Communication::Message message = Communication::Reader::GetInstance().GetMessage();
        m_index = (StopType_e) message.m_header.m_index;

        switch (m_index)
        {
            case StopType_e::SET_PAUSE:
            {
                if (message.m_body.m_bufferSize == 1)
                {
                    m_pauseTimer = millis();
                    m_pauseInterval = message.m_body.m_variables[0].toInt();
                }
                else
                {
                    INFO("To few arguments for stop request",String((uint16_t)message.m_body.m_bufferSize));
                }
            }
            break;
            case StopType_e::STOP_ROBOT:
            {
                Base::Angles position{IDLE_POS_SEGMENT_00,
                                      IDLE_POS_SEGMENT_01,
                                      IDLE_POS_SEGMENT_02,
                                      IDLE_POS_SEGMENT_03};
                Robot::GetInstance().SetTarget(position);
            }
            break;
            case StopType_e::ESTOP_ROBOT:
            default:
                INFO("Undefined stop type, should not happen",String((uint16_t) m_index));
                break;
        }
    }
    void StateStop::DoActivity()
    {
        switch (m_index)
        {
            case StopType_e::SET_PAUSE:
            {
                if (millis() - m_pauseTimer >= m_pauseInterval)
                {
                    m_activityComplete = true;
                }
            }
            break;
            case StopType_e::STOP_ROBOT:
            {
                if (Robot::GetInstance().IsMoveComplete())
                {
                    Robot::GetInstance().DisableRobot();
                    m_machine.ScheduleReset();
                    m_activityComplete = true;
                }
            }
            break;
            case StopType_e::ESTOP_ROBOT:
            default:
                break;
        }
    }
    void StateStop::ExitAction()
    {
    }
}// namespace Base