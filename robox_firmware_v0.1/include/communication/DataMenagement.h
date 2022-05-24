// ======================================
// Filename:    DataManagement.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_CONTROLLER_DATA_COMMUNICATION_MENAGEMENT_H
#define ROBOX_CONTROLLER_DATA_COMMUNICATION_MENAGEMENT_H

#include <Arduino.h>
#include "Message.h"

namespace Communication
{
    class DataManagement
    {
    public:
        /**
         * @brief Set the speed and acceleration of the robot joints.
         * 
         * @param message 
         */
        static void SetSpeedAccel(const Communication::Message& message);
        /**
         * @brief Set the verbosity levels for the application.
         * 
         * @param message 
         */
        static void SetVerbosity(const Communication::Message& message);
        /**
         * @brief Set the frame offset of the Robot.
         * 
         * @param message 
         */
        static void SetFrameOffset(const Communication::Message& message);
        /**
         * @brief Set the gripper offset of the Robot.
         * 
         * @param message 
         */
        static void SetGripperOffset(const Communication::Message& message);
        /**
         * @brief Set the idle positions of the Robot.
         * 
         * @param message 
         */
        static void SetIdlePositions(const Communication::Message& message);
        /**
         * @brief Get the dynamic data of the application.
         * 
         * @see message_protocol.md for more information.
         * 
         * @param message 
         */
        static void GetDynamicData(const Communication::Message& message);
        /**
         * @brief Get the static data of the application.
         * 
         * @see message_protocol.md for more information.
         * 
         * @param message 
         */
        static void GetStaticData(const Communication::Message& message);
        /**
         * @brief Get the run time data of the application.
         * 
         * @see message_protocol.md for more information.
         * 
         * @param message 
         */
        static void GetRunTimeData(const Communication::Message& message);
    };
}// namespace Communication

#endif// ROBOX_CONTROLLER_DATA_COMMUNICATION_MENAGEMENT_H