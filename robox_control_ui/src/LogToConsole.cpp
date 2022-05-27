// ======================================
// Filename:    LogToConsole.cpp
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        15-05-2022
// ======================================

#include "LogToConsole.h"
#include "Logger.h"
#include "String.h"

namespace Logger
{
    void LogToConsole::Log(const LogMessage& message)
    {
        Utils::Color_e textColor = (static_cast<SourceType_e>(message.m_sourceType) == SourceType_e::EXTERNAL) ? Utils::Color_e::BRIGHT_BLUE : Utils::Color_e::BRIGHT_WHITE;


        std::string timestamp = message.m_timestamp;
        std::string stackTrace = message.m_filename + ":" + message.m_lineNumber +" "+ message.m_func;

        std::string type = CreateTypeString(message.m_verbosityType);
        std::string text = message.m_message;
        std::string varString = (message.m_varString == "n/a") ? "" : message.m_varString;

        timestamp = Utils::String::Color(timestamp, Utils::Color_e::BRIGHT_BLACK);
        stackTrace = Utils::String::Color(stackTrace, Utils::Color_e::BRIGHT_BLACK);
        if (!varString.empty())
        {
            varString = Utils::String::Color(varString, textColor);
        }

        text = Utils::String::Color(text, textColor);


        Utils::String::Encase(timestamp, '[', ']', textColor);
        Utils::String::Encase(stackTrace, '[', ']', textColor);

        if (!varString.empty())
        {
            Utils::String::Encase(varString, '(', ')', Utils::Color_e::BRIGHT_CYAN);
        }

        std::string logString;
        logString += timestamp;
        logString += type + "\t";
        logString += text + " ";
        logString += varString;
        logString += stackTrace;

        std::cout << logString << std::endl;
    }
    std::string LogToConsole::CreateTypeString(VerbosityType_e type)
    {
        std::string typeString;
        Utils::Color_e color = Utils::Color_e::NONE;

        switch (type)
        {
            case VerbosityType_e::ERROR:
                typeString = "[ERROR]";
                color = Utils::Color_e::BRIGHT_RED;
                break;
            case VerbosityType_e::WARNING:
                typeString = "[WARNING]";
                color = Utils::Color_e::BRIGHT_YELLOW;
                break;
            case VerbosityType_e::INFO:
                typeString = "[INFO] ";
                color = Utils::Color_e::BRIGHT_GREEN;
                break;
            case VerbosityType_e::DEBUG:
                typeString = "[DEBUG]";
                color = Utils::Color_e::BRIGHT_MAGENTA;
                break;
            default:
                ERROR("Undefined severity", static_cast<uint16_t>(type));
                typeString = "[UNDEFINED]";
                break;
        }
        typeString = Utils::String::Color(typeString, color);
        return typeString;
    }
}// namespace Utils