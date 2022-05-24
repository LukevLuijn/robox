// ======================================
// Filename:    Reader.cpp
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#include "communication/Reader.h"
#include "communication/DataMenagement.h"

#include "base/Robot.h"
#include "utils/Logger.h"

namespace Communication
{
    void Reader::Run()
    {
        ReadSerial();
    }
    Message Reader::GetMessage()
    {
        if (!m_messageQueue.empty())
        {
            Message message = m_messageQueue.front();
            m_messageQueue.pop();
            return message;
        }
        return Message("00[]");
    }
    MessageType_e Reader::GetMessageType()
    {
        MessageType_e type = MessageType_e::UNDEFINED;
        if (!m_messageQueue.empty())
        {
            type = m_messageQueue.front().m_header.m_type;
        }
        return type;
    }
    void Reader::WriteMessage(const String& msgString)
    {
        String message = RESPONSE_START_CHAR + msgString + RESPONSE_STOP_CHAR;
        Utils::Logger::GetInstance().DirectWrite(message);
    }
    bool Reader::QueueEmpty()
    {
        return m_messageQueue.empty();
    }
    void Reader::ClearMessageQueue()
    {
        MessageQueue empty;
        std::swap(m_messageQueue, empty);

        DEBUG("Message queue emptied");
    }
    void Reader::SortMessages(const String& string)
    {
        Message message(string);

        if (!CheckDataMessage(message))
        {
            if (message.m_header.m_type == MessageType_e::STOP &&
                message.m_header.m_index == (uint8_t) StopType_e::ESTOP_ROBOT)
            {
                Base::Robot::GetInstance().DisableRobot();
                ClearMessageQueue();
                m_messageQueue.push(Message("R0[]"));
            }
            else
            {
                m_messageQueue.push(message);
            }
        }
    }
    void Reader::ReadSerial()
    {
        bool newMessage = false;
        static bool inProgress = false;
        static byte ndx = 0;

        char readChar;

        while ((Serial.available() > 0) && !newMessage)
        {
            readChar = Serial.read();

            if (inProgress)
            {
                if (readChar != MESSAGE_STOP_CHAR)
                {
                    m_reading[ndx] = readChar;
                    ++ndx;

                    if (ndx >= MESSAGE_SIZE)
                    {
                        ndx = MESSAGE_SIZE - 1;
                    }
                }
                else
                {
                    m_reading[ndx] = '\0';
                    inProgress = false;
                    ndx = 0;

                    SortMessages(m_reading);
                }
            }
            else if (readChar == MESSAGE_START_CHAR)
            {
                inProgress = true;
            }
        }
    }
    bool Reader::CheckDataMessage(const Message& message)
    {
        switch (message.m_header.m_type)
        {
            case MessageType_e::START:
            case MessageType_e::MOVE:
            case MessageType_e::STOP:
            case MessageType_e::RESET:
            {
                return false;
            }
            break;
            case MessageType_e::DATA:
            {
                HandleDataMessage(message);
                return true;
            }
            break;
            case MessageType_e::UNDEFINED:
            default:
                ERROR("Message type undefined", String((uint16_t) message.m_header.m_type));
                return true;// message is ignored
                break;
        }
    }
    void Reader::HandleDataMessage(const Message& message)
    {
        switch (message.m_header.m_type)
        {
            case MessageType_e::DATA:
            {
                switch ((DataType_e) message.m_header.m_index)
                {
                    case DataType_e::GET_RUNTIME:
                        Communication::DataManagement::GetRunTimeData(message);
                        break;
                    case DataType_e::GET_DYNAMIC:
                        Communication::DataManagement::GetDynamicData(message);
                        break;
                    case DataType_e::GET_STATIC:
                        Communication::DataManagement::GetStaticData(message);
                        break;
                    case DataType_e::SET_SPEED_ACCEL:
                        Communication::DataManagement::SetSpeedAccel(message);
                        break;
                    case DataType_e::SET_VERBOSITY:
                        Communication::DataManagement::SetVerbosity(message);
                        break;
                    case DataType_e::SET_FRAME_OFFSET:
                        Communication::DataManagement::SetFrameOffset(message);
                        break;
                    case DataType_e::SET_GRIP_OFFSET:
                        Communication::DataManagement::SetGripperOffset(message);
                        break;
                    case DataType_e::SET_IDLE_POSITIONS:
                        Communication::DataManagement::SetIdlePositions(message);
                        break;
                    case DataType_e::HOMING_COMPLETE:
                        break;
                }
            }
            break;
            case MessageType_e::START:
            case MessageType_e::MOVE:
            case MessageType_e::STOP:
            case MessageType_e::RESET:
            case MessageType_e::UNDEFINED:
            default:
                ERROR("Message type error, should not happen", String((uint16_t) message.m_header.m_type));
                break;
        }
    }

}// namespace Communication
