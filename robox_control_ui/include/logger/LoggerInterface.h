// ======================================
// Filename:    LoggerInterface.h
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        15-05-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_LOGGER_INTERFACE_H
#define ROBOX_CONTROL_UI_LOGGER_INTERFACE_H

#define USING_WIDGETS true

#include <array>
#include <string>

#include "msg_protocol.h"
#include "LogMessage.h"

namespace Logger
{
    class ILogger
    {
    public:
        ILogger() = default;
        virtual ~ILogger() = default;
        /**
         * @brief Log a message of type LogMessage.
         * 
         * @param message 
         */
        virtual void Log(const LogMessage& message) = 0;
    };
}// namespace Interfaces

#endif//ROBOX_USER_INTERFACE_LOGGER_INTERFACE_H
