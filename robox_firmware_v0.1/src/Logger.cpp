// ======================================
// Filename:    Logger.cpp
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#include "utils/Logger.h"

namespace Utils
{
    void Logger::DirectWrite(const String& message)
    {
        Serial.write(message.c_str());
        Serial.send_now();
    }
    void Logger::Error(const String& message, const String& caller, const String& file, uint16_t lineNum, const String& variable)
    {
        Print(VerbosityType_e::ERROR, message, variable, caller, file, lineNum);
    }
    void Logger::Warning(const String& message, const String& caller, const String& file, uint16_t lineNum, const String& variable)
    {
        Print(VerbosityType_e::WARNING, message, variable, caller, file, lineNum);
    }
    void Logger::Info(const String& message, const String& caller, const String& file, uint16_t lineNum, const String& variable)
    {
        Print(VerbosityType_e::INFO, message, variable, caller, file, lineNum);
    }
    void Logger::Debug(const String& message, const String& caller, const String& file, uint16_t lineNum, const String& variable)
    {
        Print(VerbosityType_e::DEBUG, message, variable, caller, file, lineNum);
    }
    void Logger::Transition(const String& from, const String& to, const String& caller, const String& file, uint16_t lineNum)
    {
        String message = from + " -> " + to;
        Print(VerbosityType_e::INFO, message, "", caller, file, lineNum);
    }
    void Logger::Print(VerbosityType_e verbosity, const String& message, const String& variable, const String& caller,
                   const String& file, uint16_t lineNum)
    {
        if (m_whiteList[(size_t) verbosity])
        {
            String log = String((uint16_t)verbosity);
            log.append("[").append(message);
            log.append(";").append(caller);
            log.append(";").append(file);
            log.append(";").append(String(lineNum));
            log.append(";").append((variable == "") ? "n/a" : variable);
            log.append("]");

            WriteLogMessage(log);
        }
    }
    void Logger::SetVerbosity(bool error, bool warning, bool info, bool debug)
    {
        Whitelist newList = {error, warning, info, debug};
        for (size_t i = 0; i < VERBOSITY_LEVELS; ++i)
        {
            m_whiteList[i] = newList[i];
        }
        PrintVerbosity();
    }
    void Logger::EnableError()
    {
        m_whiteList[(size_t) VerbosityType_e::ERROR] = true;
        PrintVerbosity();
    }
    void Logger::DisableError()
    {
        m_whiteList[(size_t) VerbosityType_e::ERROR] = false;
        PrintVerbosity();
    }
    void Logger::EnableWarning()
    {
        m_whiteList[(size_t) VerbosityType_e::WARNING] = true;
        PrintVerbosity();
    }
    void Logger::DisableWarning()
    {
        m_whiteList[(size_t) VerbosityType_e::WARNING] = false;
        PrintVerbosity();
    }
    void Logger::EnableInfo()
    {
        m_whiteList[(size_t) VerbosityType_e::INFO] = true;
        PrintVerbosity();
    }
    void Logger::DisableInfo()
    {
        m_whiteList[(size_t) VerbosityType_e::INFO] = false;
        PrintVerbosity();
    }
    void Logger::EnableDebug()
    {
        m_whiteList[(size_t) VerbosityType_e::DEBUG] = true;
        PrintVerbosity();
    }
    void Logger::DisableDebug()
    {
        m_whiteList[(size_t) VerbosityType_e::DEBUG] = false;
        PrintVerbosity();
    }
    bool Logger::GetVerbosityError()
    {
        return m_whiteList[(uint8_t) VerbosityType_e::ERROR];
    }
    bool Logger::GetVerbosityWarning()
    {
        return m_whiteList[(uint8_t) VerbosityType_e::WARNING];
    }
    bool Logger::GetVerbosityInfo()
    {
        return m_whiteList[(uint8_t) VerbosityType_e::INFO];
    }
    bool Logger::GetVerbosityDebug()
    {
        return m_whiteList[(uint8_t) VerbosityType_e::DEBUG];
    }
    void Logger::PrintVerbosity()
    {
        String errorStr = "Error:" + (m_whiteList[(uint8_t) VerbosityType_e::ERROR]) ? "true" : "false";
        String warningStr = "Warning:" + (m_whiteList[(uint8_t) VerbosityType_e::WARNING]) ? "true" : "false";
        String infoStr = "Info:" + (m_whiteList[(uint8_t) VerbosityType_e::INFO]) ? "true" : "false";
        String debugStr = "Debug:" + (m_whiteList[(uint8_t) VerbosityType_e::DEBUG]) ? "true" : "false";
        String verbString = errorStr + "," + warningStr + ","  + infoStr + "," + debugStr;
        DEBUG("Verbosity whitelist edit", verbString);
    }
    void Logger::WriteLogMessage(const String& message)
    {
        String msgString = LOG_START_CHAR + message + LOG_STOP_CHAR;
        DirectWrite(msgString);
    }

}// namespace Utils