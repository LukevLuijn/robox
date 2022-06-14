// ======================================
// Filename:    DataManager.cpp
// Project:     ROBOX Control UI
// Subsystem:   Main Application
// Author:      Luke van Luijn
// Data:        15-05-2022
// ======================================

#include "DataManager.h"
#include "String.h"
#include "msg_protocol.h"

namespace Driver
{
    void DataManager::ParseMessage(const std::string& message)
    {
        auto type = static_cast<MessageType_e>(message[0]);
        auto index = static_cast<DataType_e>(message[1] - '0');

        if (type == MessageType_e::DATA && index !=DataType_e::HOMING_COMPLETE)
        {
            std::string params = message.substr(2);                                 // remove message header
            Utils::String::Remove(params, std::vector<char>{'[', ']'});             // remove encasement
            std::vector<std::string> paramList = Utils::String::Divide(params, ',');// separate values by ','

            switch (index)
            {
                case DataType_e::GET_DYNAMIC:
                    ParseDynamicData(paramList);
                    break;
                case DataType_e::GET_RUNTIME:
                    ParseRunTimeData(paramList);
                    break;
                case DataType_e::GET_STATIC:
                    ParseStaticData(paramList);
                    break;
                case DataType_e::HOMING_COMPLETE:
                case DataType_e::SET_SPEED_ACCEL:   // ignored
                case DataType_e::SET_VERBOSITY:     // ignored
                case DataType_e::SET_FRAME_OFFSET:  // ignored
                case DataType_e::SET_GRIP_OFFSET:   // ignored
                case DataType_e::SET_IDLE_POSITIONS:// ignored
//                    ERROR("\'SET\' type detected, should not happen");
                    break;
                default:
//                    ERROR("Undefined DataType_e, should not happen", message.substr(0, 2));
                    break;
            }
        }
    }
    void DataManager::ParseRunTimeData(const std::vector<std::string>& data)
    {
        if (data.size() == RUNTIME_DATA_SIZE)
        {
            m_segment00.m_currentPosition = std::stof(data[static_cast<size_t>(RunTimeData_e::CURRENT_POS_00)]);
            m_segment01.m_currentPosition = std::stof(data[static_cast<size_t>(RunTimeData_e::CURRENT_POS_01)]);
            m_segment02.m_currentPosition = std::stof(data[static_cast<size_t>(RunTimeData_e::CURRENT_POS_02)]);
            m_segment03.m_currentPosition = std::stof(data[static_cast<size_t>(RunTimeData_e::CURRENT_POS_03)]);

            m_segment00.m_IsActive = data[static_cast<size_t>(RunTimeData_e::IS_RUNNING_00)] == "true";
            m_segment01.m_IsActive = data[static_cast<size_t>(RunTimeData_e::IS_RUNNING_01)] == "true";
            m_segment02.m_IsActive = data[static_cast<size_t>(RunTimeData_e::IS_RUNNING_02)] == "true";
            m_segment03.m_IsActive = data[static_cast<size_t>(RunTimeData_e::IS_RUNNING_03)] == "true";

            m_gripperPosition[0] = std::stof(data[static_cast<size_t>(RunTimeData_e::POSITION_X)]);
            m_gripperPosition[1] = std::stof(data[static_cast<size_t>(RunTimeData_e::POSITION_Y)]);
            m_gripperPosition[2] = std::stof(data[static_cast<size_t>(RunTimeData_e::POSITION_Z)]);

            m_gripperPWM00 = static_cast<uint8_t>(std::stoi(data[static_cast<size_t>(RunTimeData_e::GRIPPER_SIG_00)]));
            m_gripperPWM01 = static_cast<uint8_t>(std::stoi(data[static_cast<size_t>(RunTimeData_e::GRIPPER_SIG_01)]));
        }
        else
        {
            std::vector<size_t> vars = {data.size(), RUNTIME_DATA_SIZE};
//            ERROR("Invalid buffer size", vars);
        }
    }
    void DataManager::ParseDynamicData(const std::vector<std::string>& data)
    {
        if (data.size() == DYNAMIC_DATA_SIZE)
        {
            m_segment00.m_currentSpeed = std::stof(data[static_cast<size_t>(DynamicData_e::CURRENT_SPEED_00)]);
            m_segment01.m_currentSpeed = std::stof(data[static_cast<size_t>(DynamicData_e::CURRENT_SPEED_01)]);
            m_segment02.m_currentSpeed = std::stof(data[static_cast<size_t>(DynamicData_e::CURRENT_SPEED_02)]);
            m_segment03.m_currentSpeed = std::stof(data[static_cast<size_t>(DynamicData_e::CURRENT_SPEED_03)]);

            m_segment00.m_currentAccel = std::stof(data[static_cast<size_t>(DynamicData_e::CURRENT_ACCEL_00)]);
            m_segment01.m_currentAccel = std::stof(data[static_cast<size_t>(DynamicData_e::CURRENT_ACCEL_01)]);
            m_segment02.m_currentAccel = std::stof(data[static_cast<size_t>(DynamicData_e::CURRENT_ACCEL_02)]);
            m_segment03.m_currentAccel = std::stof(data[static_cast<size_t>(DynamicData_e::CURRENT_ACCEL_03)]);

            m_segment00.m_idlePosition = std::stof(data[static_cast<size_t>(DynamicData_e::IDLE_POS_SEGMENT_00)]);
            m_segment01.m_idlePosition = std::stof(data[static_cast<size_t>(DynamicData_e::IDLE_POS_SEGMENT_01)]);
            m_segment02.m_idlePosition = std::stof(data[static_cast<size_t>(DynamicData_e::IDLE_POS_SEGMENT_02)]);
            m_segment03.m_idlePosition = std::stof(data[static_cast<size_t>(DynamicData_e::IDLE_POS_SEGMENT_03)]);

            m_verbosityError = data[static_cast<size_t>(DynamicData_e::VERBOSITY_ERROR)] == "true";
            m_verbosityWarning = data[static_cast<size_t>(DynamicData_e::VERBOSITY_WARNING)] == "true";
            m_verbosityInfo = data[static_cast<size_t>(DynamicData_e::VERBOSITY_INFO)] == "true";
            m_verbosityDebug = data[static_cast<size_t>(DynamicData_e::VERBOSITY_DEBUG)] == "true";

            m_frameHeightOffset = std::stof(data[static_cast<size_t>(DynamicData_e::FRAME_HEIGHT_OFFSET)]);
            m_gripperHeightOffset = std::stof(data[static_cast<size_t>(DynamicData_e::GRIPPER_HEIGHT_OFFSET)]);
            m_actualZeroPosSeg00 = std::stof(data[static_cast<size_t>(DynamicData_e::ACTUAL_ZERO_POS_SEG_00)]);
        }
        else
        {
            std::vector<size_t> vars = {data.size(), DYNAMIC_DATA_SIZE};
//            ERROR("Invalid buffer size", vars);
        }
    }
    void DataManager::ParseStaticData(const std::vector<std::string>& data)
    {
        if (data.size() == STATIC_DATA_SIZE)
        {
            m_segment00.m_stepPin =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MOTOR_STP_00)]));
            m_segment00.m_directionPin =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MOTOR_DIR_00)]));
            m_segment00.m_enablePin =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MOTOR_ENB_00)]));
            m_segment00.m_hallSensorPin =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::HALL_SENSOR_00)]));
            m_segment00.m_maxPosition =
                    static_cast<int32_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MAX_HEIGHT_SEGMENT_00)]));
            m_segment00.m_minPosition =
                    static_cast<int32_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MIN_HEIGHT_SEGMENT_00)]));
            m_segment00.m_maxSpeed =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MAX_SPEED_SEGMENT_00)]));
            m_segment00.m_maxAccel =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MAX_ACCEL_SEGMENT_00)]));
            m_segment00.m_microStepping =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MICRO_STEPPING_00)]));
            m_segment00.m_stepsPerUnit = std::stof(data[static_cast<size_t>(StaticData_e::STEPS_PER_MILLIMETER_00)]);

            m_segment01.m_stepPin =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MOTOR_STP_01)]));
            m_segment01.m_directionPin =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MOTOR_DIR_01)]));
            m_segment01.m_enablePin =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MOTOR_ENB_01)]));
            m_segment01.m_hallSensorPin =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::HALL_SENSOR_01)]));
            m_segment01.m_maxPosition =
                    static_cast<int32_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MAX_ANGLE_SEGMENT_01)]));
            m_segment01.m_minPosition =
                    static_cast<int32_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MIN_ANGLE_SEGMENT_01)]));
            m_segment01.m_maxSpeed =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MAX_SPEED_SEGMENT_01)]));
            m_segment01.m_maxAccel =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MAX_ACCEL_SEGMENT_01)]));
            m_segment01.m_microStepping =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MICRO_STEPPING_01)]));
            m_segment01.m_stepsPerUnit = std::stof(data[static_cast<size_t>(StaticData_e::STEPS_PER_DEGREE_01)]);

            m_segment02.m_stepPin =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MOTOR_STP_02)]));
            m_segment02.m_directionPin =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MOTOR_DIR_02)]));
            m_segment02.m_enablePin =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MOTOR_ENB_02)]));
            m_segment02.m_hallSensorPin =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::HALL_SENSOR_02)]));
            m_segment02.m_maxPosition =
                    static_cast<int32_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MAX_ANGLE_SEGMENT_02)]));
            m_segment02.m_minPosition =
                    static_cast<int32_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MIN_ANGLE_SEGMENT_02)]));
            m_segment02.m_maxSpeed =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MAX_SPEED_SEGMENT_02)]));
            m_segment02.m_maxAccel =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MAX_ACCEL_SEGMENT_02)]));
            m_segment02.m_microStepping =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MICRO_STEPPING_02)]));
            m_segment02.m_stepsPerUnit = std::stof(data[static_cast<size_t>(StaticData_e::STEPS_PER_DEGREE_02)]);

            m_segment03.m_stepPin =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MOTOR_STP_03)]));
            m_segment03.m_directionPin =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MOTOR_DIR_03)]));
            m_segment03.m_enablePin =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MOTOR_ENB_03)]));
            m_segment03.m_hallSensorPin =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::HALL_SENSOR_03)]));
            m_segment03.m_maxPosition =
                    static_cast<int32_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MAX_ANGLE_SEGMENT_03)]));
            m_segment03.m_minPosition =
                    static_cast<int32_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MIN_ANGLE_SEGMENT_03)]));
            m_segment03.m_maxSpeed =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MAX_SPEED_SEGMENT_03)]));
            m_segment03.m_maxAccel =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MAX_ACCEL_SEGMENT_03)]));
            m_segment03.m_microStepping =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::MICRO_STEPPING_03)]));
            m_segment03.m_stepsPerUnit = std::stof(data[static_cast<size_t>(StaticData_e::STEPS_PER_DEGREE_03)]);

            m_gripperPin00 =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::GRIPPER_SIGNAL_00)]));
            m_gripperPin01 =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::GRIPPER_SIGNAL_01)]));
            m_lengthSegment01 =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::LENGTH_SEGMENT_01)]));
            m_lengthSegment02 =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::LENGTH_SEGMENT_02)]));
            m_leadScrewPitch =
                    static_cast<uint16_t>(std::stoi(data[static_cast<size_t>(StaticData_e::LEAD_SCREW_PITCH)]));
        }
        else
        {
            std::vector<size_t> vars = {data.size(), STATIC_DATA_SIZE};
//            ERROR("Invalid buffer size", vars);
        }
    }
    [[maybe_unused]] void DataManager::Print()
    {
        auto printSegmentData = [](const SegmentData& data) {
            std::cout << " step pin:\t" << data.m_stepPin << std::endl;
            std::cout << " direction pin:\t" << data.m_directionPin << std::endl;
            std::cout << " enable pin:\t" << data.m_enablePin << std::endl;
            std::cout << " hall sensor pin:\t" << data.m_hallSensorPin << std::endl;

            std::cout << " micro stepping:\t" << data.m_microStepping << std::endl;
            std::cout << " max position:\t" << data.m_maxPosition << std::endl;
            std::cout << " min position:\t" << data.m_minPosition << std::endl;
            std::cout << " max speed:\t" << data.m_maxSpeed << std::endl;
            std::cout << " max accel:\t" << data.m_maxAccel << std::endl;
            std::cout << " steps per unit:\t" << data.m_stepsPerUnit << std::endl;

            std::cout << " idle position:\t" << data.m_idlePosition << std::endl;
            std::cout << " current speed:\t" << data.m_currentSpeed << std::endl;
            std::cout << " current accel:\t" << data.m_currentAccel << std::endl;

            std::cout << " current position:\t" << data.m_currentPosition << std::endl;
            std::cout << " currently active:\t" << ((data.m_IsActive) ? "true" : "false") << std::endl;
        };

        std::cout << "SEGMENT 00" << std::endl;
        printSegmentData(m_segment00);
        std::cout << "SEGMENT 01" << std::endl;
        printSegmentData(m_segment01);
        std::cout << "SEGMENT 02" << std::endl;
        printSegmentData(m_segment02);
        std::cout << "SEGMENT 03" << std::endl;
        printSegmentData(m_segment03);

        std::cout << "GRIPPER POSITION" << std::endl;

        std::cout << " position X:\t" << m_gripperPosition[0] << std::endl;
        std::cout << " position Y:\t" << m_gripperPosition[1] << std::endl;
        std::cout << " position Z:\t" << m_gripperPosition[2] << std::endl;

        std::cout << "GRIPPER DATA" << std::endl;

        std::cout << "gripper pin 00:\t" << m_gripperPin00 << std::endl;
        std::cout << "gripper pin 01:\t" << m_gripperPin01 << std::endl;

        std::cout << "gripper PWM 00:\t" << m_gripperPWM00 << std::endl;
        std::cout << "gripper PWM 01:\t" << m_gripperPWM01 << std::endl;

        std::cout << "VERBOSITY DATA" << std::endl;

        std::cout << "Error:\t" << ((m_verbosityError) ? "true" : "false") << std::endl;
        std::cout << "Warning:\t" << ((m_verbosityWarning) ? "true" : "false") << std::endl;
        std::cout << "Info:\t" << ((m_verbosityInfo) ? "true" : "false") << std::endl;
        std::cout << "Debug:\t" << ((m_verbosityDebug) ? "true" : "false") << std::endl;

        std::cout << "HARDWARE DATA" << std::endl;

        std::cout << "Frame height offset:\t" << m_frameHeightOffset << std::endl;
        std::cout << "Gripper height offset:\t" << m_gripperHeightOffset << std::endl;
        std::cout << "Actual zero pos seg 00:\t" << m_actualZeroPosSeg00 << std::endl;

        std::cout << "Length segment 01:\t" << m_lengthSegment01 << std::endl;
        std::cout << "Length segment 02:\t" << m_lengthSegment02 << std::endl;
        std::cout << "Lead screw pitch:\t" << m_leadScrewPitch << std::endl;
    }

}// namespace Driver
