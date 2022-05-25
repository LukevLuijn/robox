// ======================================
// Filename:    DataManagement.cpp
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#include "communication/DataMenagement.h"
#include "base/Robot.h"
#include "communication/Reader.h"

#include "config/config.h"
#include "utils/Logger.h"

namespace Communication
{
    void DataManagement::SetSpeedAccel(const Message& message)
    {
        uint8_t expectedBufferSize = AMOUNT_OF_AXES * 2;
        if (message.m_body.m_bufferSize == expectedBufferSize)
        {
            Base::Robot::GetInstance().SetSpeed(Base::Joint_e::Z0, message.m_body.m_variables[0].toFloat());
            Base::Robot::GetInstance().SetSpeed(Base::Joint_e::A1, message.m_body.m_variables[1].toFloat());
            Base::Robot::GetInstance().SetSpeed(Base::Joint_e::A2, message.m_body.m_variables[2].toFloat());
            Base::Robot::GetInstance().SetSpeed(Base::Joint_e::A3, message.m_body.m_variables[3].toFloat());

            Base::Robot::GetInstance().SetAccel(Base::Joint_e::Z0, message.m_body.m_variables[4].toFloat());
            Base::Robot::GetInstance().SetAccel(Base::Joint_e::A1, message.m_body.m_variables[5].toFloat());
            Base::Robot::GetInstance().SetAccel(Base::Joint_e::A2, message.m_body.m_variables[6].toFloat());
            Base::Robot::GetInstance().SetAccel(Base::Joint_e::A3, message.m_body.m_variables[7].toFloat());
        }
        else if (message.m_body.m_bufferSize == 2)
        {
            Base::Robot::GetInstance().SetSpeed(message.m_body.m_variables[0].toFloat());
            Base::Robot::GetInstance().SetAccel(message.m_body.m_variables[1].toFloat());
        }
        else
        {
            ERROR("Unexpected buffer size", String(message.m_body.m_bufferSize));
        }
    }
    void DataManagement::SetVerbosity(const Message& message)
    {
        uint8_t expectedBufferSize = VERBOSITY_LEVELS;
        if (message.m_body.m_bufferSize == expectedBufferSize)
        {
            bool verbosities[VERBOSITY_LEVELS];
            for (uint8_t i = 0; i < expectedBufferSize; ++i)
            {
                verbosities[i] = (message.m_body.m_variables[i] == "true") ? true : false;
            }
            Utils::Logger::GetInstance().SetVerbosity(verbosities[0], verbosities[1], verbosities[2], verbosities[3]);
        }
        else if (message.m_body.m_bufferSize == 1)
        {
            bool enabled = (message.m_body.m_variables[0] == "true") ? true : false;
            Utils::Logger::GetInstance().SetVerbosity(enabled, enabled, enabled, enabled);
        }
        else
        {
            ERROR("Unexpected buffer size", String(message.m_body.m_bufferSize));
        }
    }
    void DataManagement::SetFrameOffset(const Message& message)
    {
        uint8_t expectedBufferSize = 1;
        if (message.m_body.m_bufferSize == expectedBufferSize)
        {
            FRAME_HEIGHT_OFFSET = message.m_body.m_variables[0].toFloat();

            ACTUAL_ZERO_POS_SEG_00 = MAX_HEIGHT_SEGMENT_00 + FRAME_HEIGHT_OFFSET - GRIPPER_HEIGHT_OFFSET;
            IDLE_POS_SEGMENT_00 = ACTUAL_ZERO_POS_SEG_00;
        }
        else
        {
            ERROR("Unexpected buffer size", String(message.m_body.m_bufferSize));
        }
    }
    void DataManagement::SetGripperOffset(const Message& message)
    {
        uint8_t expectedBufferSize = 1;
        if (message.m_body.m_bufferSize == expectedBufferSize)
        {
            GRIPPER_HEIGHT_OFFSET = message.m_body.m_variables[0].toFloat();

            ACTUAL_ZERO_POS_SEG_00 = MAX_HEIGHT_SEGMENT_00 + FRAME_HEIGHT_OFFSET - GRIPPER_HEIGHT_OFFSET;
            IDLE_POS_SEGMENT_00 = ACTUAL_ZERO_POS_SEG_00;
        }
        else
        {
            ERROR("Unexpected buffer size", String(message.m_body.m_bufferSize));
        }
    }
    void DataManagement::SetIdlePositions(const Message& message)
    {
        uint8_t expectedBufferSize = AMOUNT_OF_AXES;
        if (message.m_body.m_bufferSize == expectedBufferSize)
        {
            IDLE_POS_SEGMENT_00 = message.m_body.m_variables[0].toFloat();
            IDLE_POS_SEGMENT_01 = message.m_body.m_variables[0].toFloat();
            IDLE_POS_SEGMENT_02 = message.m_body.m_variables[0].toFloat();
            IDLE_POS_SEGMENT_03 = message.m_body.m_variables[0].toFloat();
        }
        else
        {
            ERROR("Unexpected buffer size", String(message.m_body.m_bufferSize));
        }
    }
    void DataManagement::GetDynamicData(const Message& message)
    {
        (void) message;

        // (speed & accel * axes) + (verbosity levels) + (frame + gripper offset) + (idle positions);
        uint8_t amountOfFields = (AMOUNT_OF_AXES * 2) + (VERBOSITY_LEVELS) + (3) + (AMOUNT_OF_AXES);
        String data[amountOfFields] = {
                Base::Robot::GetInstance().GetSpeed(Base::Joint_e::Z0),                 //  0. float - speed Z0
                Base::Robot::GetInstance().GetAccel(Base::Joint_e::Z0),                 //  1. float - accel Z0
                Base::Robot::GetInstance().GetSpeed(Base::Joint_e::A1),                 //  2. float - speed A1
                Base::Robot::GetInstance().GetAccel(Base::Joint_e::A1),                 //  3. float - accel A1
                Base::Robot::GetInstance().GetSpeed(Base::Joint_e::A2),                 //  4. float - speed A2
                Base::Robot::GetInstance().GetAccel(Base::Joint_e::A2),                 //  5. float - accel A2
                Base::Robot::GetInstance().GetSpeed(Base::Joint_e::A3),                 //  6. float - speed A3
                Base::Robot::GetInstance().GetAccel(Base::Joint_e::A3),                 //  7. float - accel A3
                ((Utils::Logger::GetInstance().GetVerbosityError()) ? "true" : "false"),//  8. String - verbosity error
                ((Utils::Logger::GetInstance().GetVerbosityWarning()) ? "true"
                                                                      : "false"),      //  9. String - verbosity warning
                ((Utils::Logger::GetInstance().GetVerbosityInfo()) ? "true" : "false"),// 10. String - verbosity info
                ((Utils::Logger::GetInstance().GetVerbosityDebug()) ? "true" : "false"),// 11. String - verbosity debug
                FRAME_HEIGHT_OFFSET,   // 12. float - height offset frame
                GRIPPER_HEIGHT_OFFSET, // 13. float - height offset gripper
                ACTUAL_ZERO_POS_SEG_00,// 14. float - actual zero pos based on gripper & frame offsets
                IDLE_POS_SEGMENT_00,   // 15. float - idle position Z0
                IDLE_POS_SEGMENT_01,   // 16. float - idle position A1
                IDLE_POS_SEGMENT_02,   // 17. float - idle position A2
                IDLE_POS_SEGMENT_03,   // 18. float - idle position A3
        };

        String msgString = (char) MessageType_e::DATA + String((uint8_t) DataType_e::GET_DYNAMIC) + "[";

        for (uint8_t i = 0; i < amountOfFields; ++i)
        {
            msgString.append(data[i]);
            msgString.append((i != amountOfFields - 1) ? "," : "");
        }

        msgString += "]";
        Reader::GetInstance().WriteMessage(msgString);
    }
    void DataManagement::GetStaticData(const Message& message)
    {
        (void) message;

        // (pin condif x18) + (min/max pos * axes) + (max speed / accel * axes) + (microstepping * axes) + (steps per unit * axes) + (length segs (2), leadscrew pitch)
        uint8_t amountOfFields =
                (18) + (AMOUNT_OF_AXES * 2) + (AMOUNT_OF_AXES * 2) + (3) + (AMOUNT_OF_AXES) + (AMOUNT_OF_AXES);
        String data[amountOfFields] = {
                (uint16_t) HALL_SENSOR_0,        //  0. uint16_t - pin hall sensor Z0
                (uint16_t) HALL_SENSOR_1,        //  1. uint16_t - pin hall sensor A1
                (uint16_t) HALL_SENSOR_2,        //  2. uint16_t - pin hall sensor A2
                (uint16_t) HALL_SENSOR_3,        //  3. uint16_t - pin hall sensor A4
                (uint16_t) MOTOR_STP_0,          //  4. uint16_t - pin step motor Z0
                (uint16_t) MOTOR_DIR_0,          //  5. uint16_t - pin direction motor Z0
                (uint16_t) MOTOR_ENB_0,          //  6. uint16_t - pin enable motor Z0
                (uint16_t) MOTOR_STP_1,          //  7. uint16_t - pin step motor A1
                (uint16_t) MOTOR_DIR_1,          //  8. uint16_t - pin direction motor A1
                (uint16_t) MOTOR_ENB_1,          //  9. uint16_t - pin enable motor A1
                (uint16_t) MOTOR_STP_2,          // 10. uint16_t - pin step motor A2
                (uint16_t) MOTOR_DIR_2,          // 11. uint16_t - pin direction motor A2
                (uint16_t) MOTOR_ENB_2,          // 12. uint16_t - pin enable motor A2
                (uint16_t) MOTOR_STP_3,          // 13. uint16_t - pin step motor A3
                (uint16_t) MOTOR_DIR_3,          // 14. uint16_t - pin direction motor A3
                (uint16_t) MOTOR_ENB_3,          // 15. uint16_t - pin enable motor A3
                (uint16_t) GRIPPER_SIGNAL_00,    // 16. uint16_t - pin gripper 00
                (uint16_t) GRIPPER_SIGNAL_01,    // 17. uint16_t - pin gripper 01
                MAX_HEIGHT_SEGMENT_00,// 18. uint16_t - max height Z0
                MIN_HEIGHT_SEGMENT_00,// 19. uint16_t - min height Z0
                MAX_ANGLE_SEGMENT_01, // 20. uint16_t - max angle A1
                MIN_ANGLE_SEGMENT_01, // 21. uint16_t - min angle A1
                MAX_ANGLE_SEGMENT_02, // 22. uint16_t - max angle A2
                MIN_ANGLE_SEGMENT_02, // 23. uint16_t - min angle A2
                MAX_ANGLE_SEGMENT_03, // 24. uint16_t - max angle A3
                MIN_ANGLE_SEGMENT_03, // 25. uint16_t - min angle A3
                (uint16_t) MAX_SPEED_SEGMENT_00, // 26. uint16_t - max speed Z0
                (uint16_t) MAX_ACCEL_SEGMENT_00, // 27. uint16_t - max accel Z0
                (uint16_t) MAX_SPEED_SEGMENT_01, // 28. uint16_t - max speed A1
                (uint16_t) MAX_ACCEL_SEGMENT_01, // 29. uint16_t - max accel A1
                (uint16_t) MAX_SPEED_SEGMENT_02, // 30. uint16_t - max speed A2
                (uint16_t) MAX_ACCEL_SEGMENT_02, // 31. uint16_t - max accel A2
                (uint16_t) MAX_SPEED_SEGMENT_03, // 32. uint16_t - max speed A3
                (uint16_t) MAX_ACCEL_SEGMENT_03, // 33. uint16_t - max accel A3
                (uint16_t) LENGTH_SEGMENT_01,    // 34. uint16_t - length segment 01
                (uint16_t) LENGTH_SEGMENT_02,    // 35. uint16_t - length segment 02
                (uint16_t) LEADSCREW_PITCH,      // 36. uint16_t - leadscrew pitch
                (uint16_t) Base::Robot::GetInstance().GetMicrostepping(
                        Base::Joint_e::Z0),// 37. uint16_t - microstepping config Z0
                (uint16_t) Base::Robot::GetInstance().GetMicrostepping(
                        Base::Joint_e::A1),// 38. uint16_t - microstepping config A1
                (uint16_t) Base::Robot::GetInstance().GetMicrostepping(
                        Base::Joint_e::A2),// 39. uint16_t - microstepping config A2
                (uint16_t) Base::Robot::GetInstance().GetMicrostepping(
                        Base::Joint_e::A3),                                   // 40. uint16_t - microstepping config A3
                String(Base::Robot::GetInstance().GetStepsPerType(Base::Joint_e::Z0),4),// 41. float - steps per milimeter
                String(Base::Robot::GetInstance().GetStepsPerType(Base::Joint_e::A1),4),// 42. float - steps per degree
                String(Base::Robot::GetInstance().GetStepsPerType(Base::Joint_e::A2),4),// 43. float - steps per degree
                String(Base::Robot::GetInstance().GetStepsPerType(Base::Joint_e::A3),4),// 44. float - steps per degree
        };

        String msgString = (char) MessageType_e::DATA + String((uint8_t) DataType_e::GET_STATIC) + "[";

        for (uint8_t i = 0; i < amountOfFields; ++i)
        {
            msgString.append(data[i]);
            msgString.append((i != amountOfFields - 1) ? "," : "");
        }

        msgString += "]";
        Reader::GetInstance().WriteMessage(msgString);
    }
    void DataManagement::GetRunTimeData(const Message& message)
    {
        (void) message;

        Base::Point position = Base::Robot::GetInstance().GetPosition();

        uint8_t amountOfFields = (AMOUNT_OF_AXES) + (AMOUNT_OF_AXES) + (3) + (2);
        String data[amountOfFields] = {
                Base::Robot::GetInstance().GetJointPosition(Base::Joint_e::Z0),// 0. float - current position Z0
                Base::Robot::GetInstance().GetJointPosition(Base::Joint_e::A1),// 1. float - current position A1
                Base::Robot::GetInstance().GetJointPosition(Base::Joint_e::A2),// 2. float - current position A2
                Base::Robot::GetInstance().GetJointPosition(Base::Joint_e::A3),// 3. float - current position A3
                (Base::Robot::GetInstance().IsRunning(Base::Joint_e::Z0) ? "true"
                                                                         : "false"),// 4. string - currently active Z0
                (Base::Robot::GetInstance().IsRunning(Base::Joint_e::A1) ? "true"
                                                                         : "false"),// 5. string - currently active A1
                (Base::Robot::GetInstance().IsRunning(Base::Joint_e::A2) ? "true"
                                                                         : "false"),// 6. string - currently active A2
                (Base::Robot::GetInstance().IsRunning(Base::Joint_e::A3) ? "true"
                                                                         : "false"),// 7. string - currently active A3
                position.X,                                                         // 8. float - gripper position X
                position.Y,                                                         // 9. float - gripper position Y
                position.Z,                                                         // 10. float - gripper position Z
                Base::Robot::GetInstance().GetGripperPWMSignal00(),// 11. uint8_t - gripper PWM value 00
                Base::Robot::GetInstance().GetGripperPWMSignal01(),// 12. uint8_t - gripper PWM value 01
        };

        String msgString = (char) MessageType_e::DATA + String((uint8_t) DataType_e::GET_RUNTIME) + "[";

        for (uint8_t i = 0; i < amountOfFields; ++i)
        {
            msgString.append(data[i]);
            msgString.append((i != amountOfFields - 1) ? "," : "");
        }

        msgString += "]";
        Reader::GetInstance().WriteMessage(msgString);
    }


}// namespace Communication