// ======================================
// Filename:    msg_protocol.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_CONTROLLER_MSG_PROTOCOL_H
#define ROBOX_CONTROLLER_MSG_PROTOCOL_H

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
enum class VerbosityType_e : uint8_t
{
    ERROR = 0,
    WARNING = 1,
    INFO = 2,
    DEBUG = 3,
};

#endif// ROBOX_CONTROLLER_MSG_PROTOCOL_H