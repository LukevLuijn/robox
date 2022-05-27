// ======================================
// Filename:    Logger.h
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        15-05-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_LOGGER_H
#define ROBOX_CONTROL_UI_LOGGER_H

#include <filesystem>
#include <memory>
#include <mutex>

#include "LoggerInterface.h"
#include "String.h"

#define EXPAND(x) x
#define GET_MACRO(_1, _2, NAME, ...) NAME

#define ERROR1(a) Logger::Logger::GetInstance().Error(a, __func__, __FILE__, __LINE__)
#define ERROR2(a, b) Logger::Logger::GetInstance().Error(a, b, __func__, __FILE__, __LINE__)
#define ERROR(...) EXPAND(GET_MACRO(__VA_ARGS__, ERROR2, ERROR1)(__VA_ARGS__))

#define WARNING1(a) Logger::Logger::GetInstance().Warning(a, __func__, __FILE__, __LINE__)
#define WARNING2(a, b) Logger::Logger::GetInstance().Warning(a, b, __func__, __FILE__, __LINE__)
#define WARNING(...) EXPAND(GET_MACRO(__VA_ARGS__, WARNING2, WARNING1)(__VA_ARGS__))

#define INFO1(a) Logger::Logger::GetInstance().Info(a, __func__, __FILE__, __LINE__)
#define INFO2(a, b) Logger::Logger::GetInstance().Info(a, b, __func__, __FILE__, __LINE__)
#define INFO(...) EXPAND(GET_MACRO(__VA_ARGS__, INFO2, INFO1)(__VA_ARGS__))

#define DEBUG1(a) Logger::Logger::GetInstance().Debug(a, __func__, __FILE__, __LINE__)
#define DEBUG2(a, b) Logger::Logger::GetInstance().Debug(a, b, __func__, __FILE__, __LINE__)
#define DEBUG(...) EXPAND(GET_MACRO(__VA_ARGS__, DEBUG2, DEBUG1)(__VA_ARGS__))

namespace Logger
{
    enum class LogType_e : uint8_t
    {
        CONSOLE = 0,
#ifdef USING_WIDGETS
        WX_PANEL = 1,
#endif
    };
    enum class LogText_e : uint8_t
    {
        MESSAGE = 0,
        FUNC = 1,
        VAR_STRING = 2,
        FILENAME = 3,
        LINE_NUMBER = 4,
    };

    typedef std::array<std::string, 5> LogText;

    class Logger
    {
    public:
        Logger(const Logger& other) = delete;
        Logger& operator=(const Logger& other) = delete;
        virtual ~Logger() = default;

        static Logger& GetInstance()
        {
            static Logger instance;
            return instance;
        }

    private:
        Logger();

    public:
        /**
         * @brief Set the type of logger.
         * 
         * @param type 
         */
        static void SetLogType(LogType_e type);
        /**
         * @brief Get the type of the current logger.
         * 
         * @return LogType_e 
         */
        [[maybe_unused]] static LogType_e GetLogType();
        /**
         * @brief Toggle the verbosity for the error logs.
         * 
         * @param enable True if errors should be logged, false if not.
         */
        [[maybe_unused]] static void SetVerbosityError(bool enable);
        /**
         * @brief Toggle the verbosity for the warning logs.
         * 
         * @param enable True if warnings should be logged, false if not.
         */
        [[maybe_unused]] static void SetVerbosityWarning(bool enable);
        /**
         * @brief Toggle the verbosity for the info logs.
         * 
         * @param enable True if info should be logged, false if not.
         */
        [[maybe_unused]] static void SetVerbosityInfo(bool enable);
        /**
         * @brief Toggle the verbosity for the debug logs.
         * 
         * @param enable True if debugs should be logged, false if not.
         */
        [[maybe_unused]] static void SetVerbosityDebug(bool enable);
        /**
         * @brief Log a message from an external source e.g. serial.
         * 
         * @param message 
         */
        void LogExternal(const std::string& message);
        /**
         * @brief Log error message.
         * 
         * @param text Text of message.
         * @param caller Caller of this method.
         * @param file File of caller.
         * @param lineNumber Line number of caller.
         */
        void Error(const std::string& text, const std::string& caller, const std::string& file, int16_t lineNumber)
        {
            LogText message;
            message[static_cast<size_t>(LogText_e::FUNC)] = caller;
            message[static_cast<size_t>(LogText_e::MESSAGE)] = text;
            message[static_cast<size_t>(LogText_e::VAR_STRING)] = "";
            message[static_cast<size_t>(LogText_e::FILENAME)] = std::filesystem::path(file).filename().generic_string();
            message[static_cast<size_t>(LogText_e::LINE_NUMBER)] = std::to_string(lineNumber);
            CreateMessage(message, VerbosityType_e::ERROR);
        }
        /**
         * @brief Log error message
         * 
         * @tparam T variable type.
         * @param text Text of message.
         * @param var Variable of message.
         * @param caller Caller of this method.
         * @param file File of caller.
         * @param lineNumber Line number of caller.
         */
        template<typename T>
        void Error(const std::string& text, T var, const std::string& caller, const std::string& file,
                   int16_t lineNumber)
        {
            LogText message;
            message[static_cast<size_t>(LogText_e::FUNC)] = caller;
            message[static_cast<size_t>(LogText_e::MESSAGE)] = text;
            message[static_cast<size_t>(LogText_e::VAR_STRING)] = CreateVarString(var);
            message[static_cast<size_t>(LogText_e::FILENAME)] = std::filesystem::path(file).filename().generic_string();
            message[static_cast<size_t>(LogText_e::LINE_NUMBER)] = std::to_string(lineNumber);
            CreateMessage(message, VerbosityType_e::ERROR);
        }
        /**
         * @brief Log error message
         * 
         * @tparam T variable type.
         * @param text Text of message.
         * @param vars List of variables of type T.
         * @param caller Caller of this method.
         * @param file File of caller.
         * @param lineNumber Line number of caller.
         */
        template<typename T>
        void Error(const std::string& text, std::vector<T>& vars, const std::string& caller, const std::string& file,
                   int16_t lineNumber)
        {
            LogText message;
            message[static_cast<size_t>(LogText_e::FUNC)] = caller;
            message[static_cast<size_t>(LogText_e::MESSAGE)] = text;
            message[static_cast<size_t>(LogText_e::VAR_STRING)] = CreateVarString(vars);
            message[static_cast<size_t>(LogText_e::FILENAME)] = std::filesystem::path(file).filename().generic_string();
            message[static_cast<size_t>(LogText_e::LINE_NUMBER)] = std::to_string(lineNumber);
            CreateMessage(message, VerbosityType_e::ERROR);
        }
        /**
         * @brief Log warning message.
         * 
         * @param text Text of message.
         * @param caller Caller of this method.
         * @param file File of caller.
         * @param lineNumber Line number of caller.
         */
        void Warning(const std::string& text, const std::string& caller, const std::string& file, int16_t lineNumber)
        {
            LogText message;
            message[static_cast<size_t>(LogText_e::FUNC)] = caller;
            message[static_cast<size_t>(LogText_e::MESSAGE)] = text;
            message[static_cast<size_t>(LogText_e::VAR_STRING)] = "";
            message[static_cast<size_t>(LogText_e::FILENAME)] = std::filesystem::path(file).filename().generic_string();
            message[static_cast<size_t>(LogText_e::LINE_NUMBER)] = std::to_string(lineNumber);
            CreateMessage(message, VerbosityType_e::WARNING);
        }
        /**
         * @brief Log warning message
         * 
         * @tparam T variable type.
         * @param text Text of message.
         * @param var Variable of message.
         * @param caller Caller of this method.
         * @param file File of caller.
         * @param lineNumber Line number of caller.
         */
        template<typename T>
        void Warning(const std::string& text, T var, const std::string& caller, const std::string& file,
                     int16_t lineNumber)
        {
            LogText message;
            message[static_cast<size_t>(LogText_e::FUNC)] = caller;
            message[static_cast<size_t>(LogText_e::MESSAGE)] = text;
            message[static_cast<size_t>(LogText_e::VAR_STRING)] = CreateVarString(var);
            message[static_cast<size_t>(LogText_e::FILENAME)] = std::filesystem::path(file).filename().generic_string();
            message[static_cast<size_t>(LogText_e::LINE_NUMBER)] = std::to_string(lineNumber);
            CreateMessage(message, VerbosityType_e::WARNING);
        }
        /**
         * @brief Log warning message
         * 
         * @tparam T variable type.
         * @param text Text of message.
         * @param vars List of variables of type T.
         * @param caller Caller of this method.
         * @param file File of caller.
         * @param lineNumber Line number of caller.
         */
        template<typename T>
        void Warning(const std::string& text, std::vector<T>& vars, const std::string& caller, const std::string& file,
                     int16_t lineNumber)
        {
            LogText message;
            message[static_cast<size_t>(LogText_e::FUNC)] = caller;
            message[static_cast<size_t>(LogText_e::MESSAGE)] = text;
            message[static_cast<size_t>(LogText_e::VAR_STRING)] = CreateVarString(vars);
            message[static_cast<size_t>(LogText_e::FILENAME)] = std::filesystem::path(file).filename().generic_string();
            message[static_cast<size_t>(LogText_e::LINE_NUMBER)] = std::to_string(lineNumber);
            CreateMessage(message, VerbosityType_e::WARNING);
        }
        /**
         * @brief Log info message.
         * 
         * @param text Text of message.
         * @param caller Caller of this method.
         * @param file File of caller.
         * @param lineNumber Line number of caller.
         */
        void Info(const std::string& text, const std::string& caller, const std::string& file, int16_t lineNumber)
        {
            LogText message;
            message[static_cast<size_t>(LogText_e::FUNC)] = caller;
            message[static_cast<size_t>(LogText_e::MESSAGE)] = text;
            message[static_cast<size_t>(LogText_e::VAR_STRING)] = "";
            message[static_cast<size_t>(LogText_e::FILENAME)] = std::filesystem::path(file).filename().generic_string();
            message[static_cast<size_t>(LogText_e::LINE_NUMBER)] = std::to_string(lineNumber);
            CreateMessage(message, VerbosityType_e::INFO);
        }
        /**
         * @brief Log info message
         * 
         * @tparam T variable type.
         * @param text Text of message.
         * @param var Variable of message.
         * @param caller Caller of this method.
         * @param file File of caller.
         * @param lineNumber Line number of caller.
         */
        template<typename T>
        void Info(const std::string& text, T var, const std::string& caller, const std::string& file,
                  int16_t lineNumber)
        {
            LogText message;
            message[static_cast<size_t>(LogText_e::FUNC)] = caller;
            message[static_cast<size_t>(LogText_e::MESSAGE)] = text;
            message[static_cast<size_t>(LogText_e::VAR_STRING)] = CreateVarString(var);
            message[static_cast<size_t>(LogText_e::FILENAME)] = std::filesystem::path(file).filename().generic_string();
            message[static_cast<size_t>(LogText_e::LINE_NUMBER)] = std::to_string(lineNumber);
            CreateMessage(message, VerbosityType_e::INFO);
        }
        /**
         * @brief Log info message
         * 
         * @tparam T variable type.
         * @param text Text of message.
         * @param vars List of variables of type T.
         * @param caller Caller of this method.
         * @param file File of caller.
         * @param lineNumber Line number of caller.
         */
        template<typename T>
        void Info(const std::string& text, std::vector<T>& vars, const std::string& caller, const std::string& file,
                  int16_t lineNumber)
        {
            LogText message;
            message[static_cast<size_t>(LogText_e::FUNC)] = caller;
            message[static_cast<size_t>(LogText_e::MESSAGE)] = text;
            message[static_cast<size_t>(LogText_e::VAR_STRING)] = CreateVarString(vars);
            message[static_cast<size_t>(LogText_e::FILENAME)] = std::filesystem::path(file).filename().generic_string();
            message[static_cast<size_t>(LogText_e::LINE_NUMBER)] = std::to_string(lineNumber);
            CreateMessage(message, VerbosityType_e::INFO);
        }
        /**
         * @brief Log debug message.
         * 
         * @param text Text of message.
         * @param caller Caller of this method.
         * @param file File of caller.
         * @param lineNumber Line number of caller.
         */
        void Debug(const std::string& text, const std::string& caller, const std::string& file, int16_t lineNumber)
        {
            LogText message;
            message[static_cast<size_t>(LogText_e::FUNC)] = caller;
            message[static_cast<size_t>(LogText_e::MESSAGE)] = text;
            message[static_cast<size_t>(LogText_e::VAR_STRING)] = "";
            message[static_cast<size_t>(LogText_e::FILENAME)] = std::filesystem::path(file).filename().generic_string();
            message[static_cast<size_t>(LogText_e::LINE_NUMBER)] = std::to_string(lineNumber);
            CreateMessage(message, VerbosityType_e::DEBUG);
        }
        /**
         * @brief Log debug message
         * 
         * @tparam T variable type.
         * @param text Text of message.
         * @param var Variable of message.
         * @param caller Caller of this method.
         * @param file File of caller.
         * @param lineNumber Line number of caller.
         */
        template<typename T>
        void Debug(const std::string& text, T var, const std::string& caller, const std::string& file,
                   int16_t lineNumber)
        {
            LogText message;
            message[static_cast<size_t>(LogText_e::FUNC)] = caller;
            message[static_cast<size_t>(LogText_e::MESSAGE)] = text;
            message[static_cast<size_t>(LogText_e::VAR_STRING)] = CreateVarString(var);
            message[static_cast<size_t>(LogText_e::FILENAME)] = std::filesystem::path(file).filename().generic_string();
            message[static_cast<size_t>(LogText_e::LINE_NUMBER)] = std::to_string(lineNumber);
            CreateMessage(message, VerbosityType_e::DEBUG);
        }
        /**
         * @brief Log debug message
         * 
         * @tparam T variable type.
         * @param text Text of message.
         * @param vars List of variables of type T.
         * @param caller Caller of this method.
         * @param file File of caller.
         * @param lineNumber Line number of caller.
         */
        template<typename T>
        void Debug(const std::string& text, std::vector<T>& vars, const std::string& caller, const std::string& file,
                   int16_t lineNumber)
        {
            LogText message;
            message[static_cast<size_t>(LogText_e::FUNC)] = caller;
            message[static_cast<size_t>(LogText_e::MESSAGE)] = text;
            message[static_cast<size_t>(LogText_e::VAR_STRING)] = CreateVarString(vars);
            message[static_cast<size_t>(LogText_e::FILENAME)] = std::filesystem::path(file).filename().generic_string();
            message[static_cast<size_t>(LogText_e::LINE_NUMBER)] = std::to_string(lineNumber);
            CreateMessage(message, VerbosityType_e::DEBUG);
        }

    private:
        template<typename T>
        [[maybe_unused]] std::string CreateVarString(T var)
        {
            return Utils::String::ToString(var);
        }
        template<typename T>
        std::string CreateVarString(const std::vector<T>& vars)
        {
            std::vector<std::string> strVars = Utils::String::ToString(vars);
            std::string varString;

            for (auto it = strVars.begin(); it != strVars.end(); ++it)
            {
                varString += *it + ((std::next(it) != strVars.end()) ? "," : "");
            }
            return varString;
        }
        void CreateMessage(LogText& text, VerbosityType_e verbosityType, SourceType_e source = SourceType_e::INTERNAL);
        void Log(const LogMessage& message);

    private:
        std::mutex m_logMutex;
        std::unique_ptr<ILogger> m_logObject;

        static LogType_e m_type;
        static bool m_whiteList[4];
    };
}// namespace Logger

#endif//ROBOX_CONTROL_UI_LOGGER_H

// https://cppcodetips.wordpress.com/2020/10/25/different-ways-to-get-the-name-of-the-calling-function-in-c/
// https://stackoverflow.com/questions/353180/how-do-i-find-the-name-of-the-calling-function
