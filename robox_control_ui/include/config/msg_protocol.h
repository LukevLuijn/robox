// ======================================
// Filename:    msg_protocol.h
// Project:     ROBOX User interface
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_USER_INTERFACE_MSG_PROTOCOL_H
#define ROBOX_USER_INTERFACE_MSG_PROTOCOL_H

#include <cstdint>

enum class MessageType_e : int8_t
{
    START = 'S',
    MOVE = 'M',
    STOP = 'E',
    DATA = 'D',
    RESET = 'R',
    UNDEFINED = 0,
};
enum class MoveType_e : uint8_t
{
    SET_INVERSE = 0,
    SET_FORWARD = 1,
    SET_GRIPPER = 2,
};
enum class StopType_e : uint8_t
{
    SET_PAUSE = 0,
    STOP_ROBOT = 1,
    ESTOP_ROBOT = 2,
};
enum class VerbosityType_e : uint8_t
{
    ERROR = 0,
    WARNING = 1,
    INFO = 2,
    DEBUG = 3,
    TRANS = 4,
};
enum class DataType_e : uint8_t
{
    GET_RUNTIME = 0,
    GET_DYNAMIC = 1,
    GET_STATIC = 2,
    SET_SPEED_ACCEL = 3,
    SET_VERBOSITY = 4,
    SET_FRAME_OFFSET = 5,
    SET_GRIP_OFFSET = 6,
    SET_IDLE_POSITIONS = 7,
    HOMING_COMPLETE = 8,
};
enum class LogParts_e : uint8_t
{
    MESSAGE = 0,
    CALLER = 1,
    FILE = 2,
    LINE_NUM = 3,
    VAR_STRING = 4,
};
enum class SourceType_e : uint8_t
{
    EXTERNAL = 0,
    INTERNAL = 1,
};
const uint8_t RUNTIME_DATA_SIZE = 13;
enum class RunTimeData_e : uint8_t
{
    CURRENT_POS_00 = 0,
    CURRENT_POS_01 = 1,
    CURRENT_POS_02 = 2,
    CURRENT_POS_03 = 3,
    IS_RUNNING_00 = 4,
    IS_RUNNING_01 = 5,
    IS_RUNNING_02 = 6,
    IS_RUNNING_03 = 7,
    POSITION_X = 8,
    POSITION_Y = 9,
    POSITION_Z = 10,
    GRIPPER_SIG_00 = 11,
    GRIPPER_SIG_01 = 12,
};
const uint8_t STATIC_DATA_SIZE = 45;
enum class StaticData_e : uint8_t
{
    HALL_SENSOR_00 = 0,
    HALL_SENSOR_01 = 1,
    HALL_SENSOR_02 = 2,
    HALL_SENSOR_03 = 3,
    MOTOR_STP_00 = 4,
    MOTOR_DIR_00 = 5,
    MOTOR_ENB_00 = 6,
    MOTOR_STP_01 = 7,
    MOTOR_DIR_01 = 8,
    MOTOR_ENB_01 = 9,
    MOTOR_STP_02 = 10,
    MOTOR_DIR_02 = 11,
    MOTOR_ENB_02 = 12,
    MOTOR_STP_03 = 13,
    MOTOR_DIR_03 = 14,
    MOTOR_ENB_03 = 15,
    GRIPPER_SIGNAL_00 = 16,
    GRIPPER_SIGNAL_01 = 17,
    MAX_HEIGHT_SEGMENT_00 = 18,
    MIN_HEIGHT_SEGMENT_00 = 19,
    MAX_ANGLE_SEGMENT_01 = 20,
    MIN_ANGLE_SEGMENT_01 = 21,
    MAX_ANGLE_SEGMENT_02 = 22,
    MIN_ANGLE_SEGMENT_02 = 23,
    MAX_ANGLE_SEGMENT_03 = 24,
    MIN_ANGLE_SEGMENT_03 = 25,
    MAX_SPEED_SEGMENT_00 = 26,
    MAX_ACCEL_SEGMENT_00 = 27,
    MAX_SPEED_SEGMENT_01 = 28,
    MAX_ACCEL_SEGMENT_01 = 29,
    MAX_SPEED_SEGMENT_02 = 30,
    MAX_ACCEL_SEGMENT_02 = 31,
    MAX_SPEED_SEGMENT_03 = 32,
    MAX_ACCEL_SEGMENT_03 = 33,
    LENGTH_SEGMENT_01 = 34,
    LENGTH_SEGMENT_02 = 35,
    LEAD_SCREW_PITCH = 36,
    MICRO_STEPPING_00 = 37,
    MICRO_STEPPING_01 = 38,
    MICRO_STEPPING_02 = 39,
    MICRO_STEPPING_03 = 40,
    STEPS_PER_MILLIMETER_00 = 41,
    STEPS_PER_DEGREE_01 = 42,
    STEPS_PER_DEGREE_02 = 43,
    STEPS_PER_DEGREE_03 = 44,
};
const uint8_t DYNAMIC_DATA_SIZE = 19;
enum class DynamicData_e : uint8_t
{
    CURRENT_SPEED_00 = 0,
    CURRENT_ACCEL_00 = 1,
    CURRENT_SPEED_01 = 2,
    CURRENT_ACCEL_01 = 3,
    CURRENT_SPEED_02 = 4,
    CURRENT_ACCEL_02 = 5,
    CURRENT_SPEED_03 = 6,
    CURRENT_ACCEL_03 = 7,
    VERBOSITY_ERROR = 8,
    VERBOSITY_WARNING = 9,
    VERBOSITY_INFO = 10,
    VERBOSITY_DEBUG = 11,
    FRAME_HEIGHT_OFFSET = 12,
    GRIPPER_HEIGHT_OFFSET = 13,
    ACTUAL_ZERO_POS_SEG_00 = 14,
    IDLE_POS_SEGMENT_00 = 15,
    IDLE_POS_SEGMENT_01 = 16,
    IDLE_POS_SEGMENT_02 = 17,
    IDLE_POS_SEGMENT_03 = 18,
};
#endif// ROBOX_USER_INTERFACE_MSG_PROTOCOL_H