//
// Created by luke on 16-05-22.
//


#ifndef ROBOX_USER_INTERFACE_LOGTOWIDGET_H
#define ROBOX_USER_INTERFACE_LOGTOWIDGET_H

#include "LoggerInterface.h"

namespace Logger
{
    class LogToWidget : public ILogger
    {
    public:
        LogToWidget() = default;
        ~LogToWidget() override = default;

        /**
         * @brief Log a message to a wxWidget output.
         * 
         * @param message 
         */
        void Log(const LogMessage& message) override;
    };
}// namespace Utils

#endif//ROBOX_USER_INTERFACE_LOGTOWIDGET_H