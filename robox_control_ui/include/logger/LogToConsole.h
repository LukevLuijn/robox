// ======================================
// Filename:    LogToConsoles.h
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        15-05-2022
// ======================================

#ifndef ROBOX_USER_INTERFACE_LOGTOCONSOLE_H
#define ROBOX_USER_INTERFACE_LOGTOCONSOLE_H

#include "LoggerInterface.h"

namespace Logger
{
    class LogToConsole : public ILogger
    {
    public:
        LogToConsole() = default;
        ~LogToConsole() override = default;

        /**
         * @brief Log a message to the console.
         * 
         * @param message 
         */
        void Log(const LogMessage& message) override;

    private:
        static std::string CreateTypeString(VerbosityType_e type);
    };
}// namespace Utils

#endif//ROBOX_USER_INTERFACE_LOGTOCONSOLE_H
