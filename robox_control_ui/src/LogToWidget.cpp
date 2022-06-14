// ======================================
// Filename:    LogToWidget.cpp
// Project:     ROBOX Control UI
// Subsystem:   Main Application
// Author:      Luke van Luijn
// Data:        16-05-2022
// ======================================

#include "LogToWidget.h"
#include "Controller.h"
#include "LogPanel.h"
#include "MainApplication.h"

namespace Logger
{
    void LogToWidget::Log(const LogMessage& message)
    {
#if USING_WIDGETS
        Widgets::LogPanel::m_messageQueue.push(message);
        auto window = dynamic_cast<Base::Controller*>(Base::TheApp().GetTopWindow());
        if (window)
        {
            window->PostLogEvent();
        }
#else
        std::cerr << "log type set to widgets but widgets are not used.." << std::endl;
        (void)message;
#endif
    }
}

