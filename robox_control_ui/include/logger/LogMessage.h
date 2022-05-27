// ======================================
// Filename:    LogMessage.h
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        17-05-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_LOG_MESSAGE_H
#define ROBOX_CONTROL_UI_LOG_MESSAGE_H

namespace Logger
{
    struct LogMessage
    {
    public:
        [[nodiscard]] std::string ToString() const
        {
            const std::string messageStr = "Message:\t\t" + m_message;
            const std::string timestamp = "Timestamp:\t" + m_timestamp;
            const std::string variables = "Variables:\t\t" + ((m_varString.empty()) ? "n/a" : m_varString);
            const std::string trace  = "Trace:\t\t\t" + m_filename + "@" + m_lineNumber + "->" + m_func;

            std::string str = messageStr + "\n" + variables + "\n" + timestamp + "\n" + trace;
            return str;
        }

        bool IsEmpty()
        {
            return m_message.empty();
        }

    public:
        std::string m_timestamp;

        std::string m_func;
        std::string m_message;
        std::string m_varString;

        std::string m_filename;
        std::string m_lineNumber;

        VerbosityType_e m_verbosityType;
        SourceType_e m_sourceType;
    };
}// namespace Utils


#endif//ROBOX_CONTROL_UI_LOGMESSAGE_H
