// ======================================
// Filename:    Logger.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_CONTROLLER_UTILS_LOGGER_CPP
#define ROBOX_CONTROLLER_UTILS_LOGGER_CPP


#define EXPAND(x) x
#define GET_MACRO(_1, _2, NAME, ...) NAME

#define ERROR1(a) Utils::Logger::GetInstance().Error(a, __func__, __FILE__, __LINE__)
#define ERROR2(a, b) Utils::Logger::GetInstance().Error(a, __func__, __FILE__, __LINE__, b)
#define ERROR(...) EXPAND(GET_MACRO(__VA_ARGS__, ERROR2, ERROR1)(__VA_ARGS__))

#define WARNING1(a) Utils::Logger::GetInstance().Warning(a, __func__, __FILE__, __LINE__)
#define WARNING2(a, b) Utils::Logger::GetInstance().Warning(a, __func__, __FILE__, __LINE__, b)
#define WARNING(...) EXPAND(GET_MACRO(__VA_ARGS__, WARNING2, WARNING1)(__VA_ARGS__))

#define INFO1(a) Utils::Logger::GetInstance().Info(a, __func__, __FILE__, __LINE__)
#define INFO2(a, b) Utils::Logger::GetInstance().Info(a, __func__, __FILE__, __LINE__, b)
#define INFO(...) EXPAND(GET_MACRO(__VA_ARGS__, INFO2, INFO1)(__VA_ARGS__))

#define DEBUG1(a) Utils::Logger::GetInstance().Debug(a, __func__, __FILE__, __LINE__)
#define DEBUG2(a, b) Utils::Logger::GetInstance().Debug(a, __func__, __FILE__, __LINE__, b)
#define DEBUG(...) EXPAND(GET_MACRO(__VA_ARGS__, DEBUG2, DEBUG1)(__VA_ARGS__))

#define TRANS(a, b) Utils::Logger::GetInstance().Transition(a, b, __func__, __FILE__, __LINE__)


#include <Arduino.h>

#include "config/config.h"
#include "config/msg_protocol.h"

namespace Utils
{
    class Logger
    {
    private:
        typedef bool Whitelist[4];

    public:
        Logger(const Logger& other) = delete;
        Logger& operator=(const Logger& other) = delete;
        virtual ~Logger() = default;
        /**
         * @brief Get logger instance.
         * 
         * @return Logger& The instance.
         */
        static Logger& GetInstance()
        {
            static Logger instance;
            return instance;
        }

    private:
        Logger() : m_whiteList{LOG_ERROR, LOG_WARNING, LOG_INFO, LOG_DEBUG}
        {
        }

    public:
        /**
         * @brief Directly write to the serial bus.
         * 
         * @param message 
         */
        void DirectWrite(const String& message);
        /**
         * @brief Print error message.
         * 
         * @param message The message to print.
         * @param caller The method who called this method (can be omitted).
         * @param variable Additional variables to print (can be omitted).
         */
        void Error(const String& message, const String& caller = "", const String& file = "", uint16_t lineNum = 0,
                   const String& variable = "");
        /**
         * @brief Print warning message. 
         * 
         * @param message The message to print.
         * @param caller The method who called this method (can be omitted).
         * @param variable Additional variables to print (can be omitted).
         */
        void Warning(const String& message, const String& caller = "", const String& file = "", uint16_t lineNum = 0,
                     const String& variable = "");
        /**
         * @brief Print info message.
         * 
         * @param message The message to print.
         * @param caller The method who called this method (can be omitted).
         * @param variable Additional variables to print (can be omitted).
         */
        void Info(const String& message, const String& caller = "", const String& file = "", uint16_t lineNum = 0,
                  const String& variable = "");
        /**
         * @brief Print debug message.
         * 
         * @param message The message to print.
         * @param caller The method who called this method (can be omitted).
         * @param variable Additional variables to print (can be omitted).
         */
        void Debug(const String& message, const String& caller = "", const String& file = "", uint16_t lineNum = 0,
                   const String& variable = "");
        /**
         * @brief Print transition message.
         * 
         * @param from Transition source.
         * @param to Transition target.
         * @param caller The method who called this method (can be omitted).
         */
        void Transition(const String& from, const String& to, const String& caller = "", const String& file = "",
                        uint16_t lineNum = 0);
        /**
         * @brief Set the logger verbosity.
         * 
         * @param error True if errors should be printed.
         * @param warning True if warnings should be printed.
         * @param info True if info's should be printed. 
         * @param debug True if debug's should be printed.
         */
        void SetVerbosity(bool error, bool warning, bool info, bool debug);
        /**
         * @brief Enable error messages.
         * 
         */
        void EnableError();
        /**
         * @brief Disable error messages.
         * 
         */
        void DisableError();
        /**
         * @brief Enable warning messages.
         * 
         */
        void EnableWarning();
        /**
         * @brief Disable warning messages.
         * 
         */
        void DisableWarning();
        /**
         * @brief Enable info messages.
         * 
         */
        void EnableInfo();
        /**
         * @brief Disable info messages.
         * 
         */
        void DisableInfo();
        /**
         * @brief Enable debug messages.
         * 
         */
        void EnableDebug();
        /**
         * @brief Disable debug messages.
         * 
         */
        void DisableDebug();
        /**
         * @brief Check if the error messages are enabled.
         * 
         * @return true If the error messages are enabled.
         * @return false If the error messages are NOT enabled.
         */
        bool GetVerbosityError();
        /**
         * @brief Check if the warning messages are enabled.
         * 
         * @return true If the warning messages are enabled.
         * @return false If the warning messages are NOT enabled.
         */
        bool GetVerbosityWarning();
        /**
         * @brief Check if the info messages are enabled.
         * 
         * @return true If the info messages are enabled.
         * @return false If the info messages are NOT enabled.
         */
        bool GetVerbosityInfo();
        /**
         * @brief Check if the debug messages are enabled.
         * 
         * @return true If the debug messages are enabled.
         * @return false If the debug messages are NOT enabled.
         */
        bool GetVerbosityDebug();

    private:
        void Print(VerbosityType_e verbosity, const String& message, const String& variable, const String& caller,
                   const String& file, uint16_t lineNum);
        void PrintVerbosity();
        void WriteLogMessage(const String& message);

    private:
        Whitelist m_whiteList;
    };
}// namespace Utils

#endif// ROBOX_CONTROLLER_UTILS_LOGGER_CPP