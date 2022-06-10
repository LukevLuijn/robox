// ======================================
// Filename:    config.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_CONTROLLER_CONFIG_H
#define ROBOX_CONTROLLER_CONFIG_H

// ROBOT HARDWARE CONFIGURATION

#define AMOUNT_OF_AXES 4

#define DEFAULT_STEPS_PER_REVOLUTION 200
#define DRIVER_MIN_PULSE_WIDTH 5

#define LENGTH_SEGMENT_01 240.0f
#define LENGTH_SEGMENT_02 150.0f

#define LEADSCREW_PITCH 8.0f

#define MAX_HEIGHT_SEGMENT_00 365.0f
#define MIN_HEIGHT_SEGMENT_00 0.0f
#define MAX_SPEED_SEGMENT_00 15000.0f
#define MAX_ACCEL_SEGMENT_00 15000.0f

#define MAX_ANGLE_SEGMENT_01 90.0f
#define MIN_ANGLE_SEGMENT_01 -90.0f
#define MAX_SPEED_SEGMENT_01 4250.0f//12000.0f
#define MAX_ACCEL_SEGMENT_01 4250.0f//12000.0f

#define MAX_ANGLE_SEGMENT_02 360.0f
#define MIN_ANGLE_SEGMENT_02 -360.0f
#define MAX_SPEED_SEGMENT_02 4250.0f//15000.0f
#define MAX_ACCEL_SEGMENT_02 4250.0f//15000.0f

#define MAX_ANGLE_SEGMENT_03 360.0f
#define MIN_ANGLE_SEGMENT_03 -360.0f
#define MAX_SPEED_SEGMENT_03 4250.0f
#define MAX_ACCEL_SEGMENT_03 4250.0f

#define HOMING_SPEED_SEGMENT_00 2750.0f//2750.0f
#define HOMING_ACCEL_SEGMENT_00 2750.0f//2750.0f

#define HOMING_SPEED_SEGMENT_01 4250.0f//4250.0f
#define HOMING_ACCEL_SEGMENT_01 4250.0f//4250.0f

#define HOMING_SPEED_SEGMENT_02 4250.0f
#define HOMING_ACCEL_SEGMENT_02 4250.0f

#define HOMING_SPEED_SEGMENT_03 4250.0f
#define HOMING_ACCEL_SEGMENT_03 4250.0f

// MOTOR PIN CONNECTIONS

#define MOTOR_STP_0 16//15
#define MOTOR_DIR_0 15//16
#define MOTOR_ENB_0 17//14

#define MOTOR_STP_1 5
#define MOTOR_DIR_1 6
#define MOTOR_ENB_1 4

#define MOTOR_STP_2 3//2
#define MOTOR_DIR_2 2//3
#define MOTOR_ENB_2 23

#define MOTOR_STP_3 8
#define MOTOR_DIR_3 7//9
#define MOTOR_ENB_3 9//7

// HALL SENSORS PIN CONNECTIONS

#define HALL_SENSOR_0 18// lift
#define HALL_SENSOR_2 20// seg 1
#define HALL_SENSOR_3 19// seg 2
#define HALL_SENSOR_1 21// gripper

// GRIPPER SIGNAL PINS

#define GRIPPER_SIGNAL_00 0
#define GRIPPER_SIGNAL_01 1

// SERIAL COMMUNICATION

#define BAUDRATE 9600
#define MESSAGE_SIZE 255

#define MESSAGE_START_CHAR '<'
#define MESSAGE_STOP_CHAR '>'

#define RESPONSE_START_CHAR '#'
#define RESPONSE_STOP_CHAR '*'

#define LOG_START_CHAR '@'
#define LOG_STOP_CHAR '*'

// SERIAL LOGGER CONFIGURATION

#define VERBOSITY_LEVELS 4

#define LOG_ERROR true
#define LOG_WARNING true
#define LOG_INFO true
#define LOG_DEBUG false

// HARDWARE OFFSETS

extern float FRAME_HEIGHT_OFFSET;
extern float GRIPPER_HEIGHT_OFFSET;
extern float IDLE_POS_SEGMENT_00;
extern float IDLE_POS_SEGMENT_01;
extern float IDLE_POS_SEGMENT_02;
extern float IDLE_POS_SEGMENT_03;
extern float ACTUAL_ZERO_POS_SEG_00;

#endif// ROBOX_CONTROLLER_CONFIG_H