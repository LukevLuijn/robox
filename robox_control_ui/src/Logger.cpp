// ======================================
// Filename:    Logger.cpp
// Project:     ROBOX Control UI
// Subsystem:   Main Application
// Author:      Luke van Luijn
// Data:        15-05-2022
// ======================================

#include "Logger.h"

#include "LogToConsole.h"
#include "LogToWidget.h"

#include "global_config.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <thread>

namespace Logger
{
    /*static*/ LogType_e Logger::m_type = LogType_e::CONSOLE;
    /*static*/ bool Logger::m_whiteList[4] = {true, true, true, true};

    Logger::Logger()
    {
        switch (m_type)
        {
            case LogType_e::CONSOLE:
            {
                m_logObject = std::make_unique<LogToConsole>();
                m_consoleLogObject = nullptr;
            }
            break;
#if USING_WIDGETS
            case LogType_e::WX_PANEL:
            {
                m_logObject = std::make_unique<LogToWidget>();
                m_consoleLogObject = std::make_unique<LogToConsole>();
            }
            break;
#endif
            default:
                break;
        }
    }
    void Logger::SetLogType(LogType_e type)
    {
        m_type = type;
    }
    LogType_e Logger::GetLogType()
    {
        return m_type;
    }
    void Logger::SetVerbosityError(bool enable)
    {
        m_whiteList[static_cast<size_t>(VerbosityType_e::ERROR)] = enable;
    }
    void Logger::SetVerbosityWarning(bool enable)
    {
        m_whiteList[static_cast<size_t>(VerbosityType_e::WARNING)] = enable;
    }
    void Logger::SetVerbosityInfo(bool enable)
    {
        m_whiteList[static_cast<size_t>(VerbosityType_e::INFO)] = enable;
    }
    void Logger::SetVerbosityDebug(bool enable)
    {
        m_whiteList[static_cast<size_t>(VerbosityType_e::DEBUG)] = enable;
    }
    void Logger::LogExternal(const std::string& message)
    {
        std::string varString = message.substr(1);
        Utils::String::RemoveEncasement(varString);

        std::vector<std::string> parts = Utils::String::Divide(varString, LOG_PART_SEPARATOR);

        LogText logText;
        logText[static_cast<size_t>(LogText_e::MESSAGE)] = parts[static_cast<size_t>(LogParts_e::MESSAGE)];
        logText[static_cast<size_t>(LogText_e::FUNC)] = parts[static_cast<size_t>(LogParts_e::CALLER)];
        logText[static_cast<size_t>(LogText_e::FILENAME)] = parts[static_cast<size_t>(LogParts_e::FILE)];
        logText[static_cast<size_t>(LogText_e::LINE_NUMBER)] = parts[static_cast<size_t>(LogParts_e::LINE_NUM)];
        logText[static_cast<size_t>(LogText_e::VAR_STRING)] = parts[static_cast<size_t>(LogParts_e::VAR_STRING)];

        auto verbosity = static_cast<VerbosityType_e>(message[0] - '0');

        CreateMessage(logText, verbosity, SourceType_e::EXTERNAL);
    }
    void Logger::CreateMessage(LogText& text, VerbosityType_e verbosityType, SourceType_e source)
    {
        if (m_whiteList[static_cast<size_t>(verbosityType)])
        {
            LogMessage message;
            {
                auto now = std::chrono::system_clock::now();
                auto in_time_t = std::chrono::system_clock::to_time_t(now);
                std::stringstream ss;
                ss << std::put_time(std::localtime(&in_time_t), "%T");
                std::string timestamp = ss.str();
                message.m_timestamp = timestamp;
            }

            message.m_verbosityType = verbosityType;
            message.m_func = text[static_cast<size_t>(LogText_e::FUNC)];
            message.m_message = text[static_cast<size_t>(LogText_e::MESSAGE)];
            message.m_varString = text[static_cast<size_t>(LogText_e::VAR_STRING)];
            message.m_filename = text[static_cast<size_t>(LogText_e::FILENAME)];
            message.m_lineNumber = text[static_cast<size_t>(LogText_e::LINE_NUMBER)];

            message.m_sourceType = source;
            Log(message);
        }
    }
    void Logger::Log(const LogMessage& message)
    {
        std::lock_guard<std::mutex> locker(m_logMutex);
        m_logObject->Log(message);
        if (m_consoleLogObject)
        {
            m_consoleLogObject->Log(message);
        }
    }

}// namespace Utils