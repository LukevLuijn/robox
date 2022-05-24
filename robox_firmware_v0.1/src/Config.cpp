// ======================================
// Filename:    Config.cpp
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#include "config/config.h"

float FRAME_HEIGHT_OFFSET = 40.0f;
float GRIPPER_HEIGHT_OFFSET = 0.0f;
float ACTUAL_ZERO_POS_SEG_00 = MAX_HEIGHT_SEGMENT_00 + FRAME_HEIGHT_OFFSET - GRIPPER_HEIGHT_OFFSET;

float IDLE_POS_SEGMENT_00 = ACTUAL_ZERO_POS_SEG_00;
float IDLE_POS_SEGMENT_01 = 90.0f;
float IDLE_POS_SEGMENT_02 = 150.0f;
float IDLE_POS_SEGMENT_03 = 150.0f;