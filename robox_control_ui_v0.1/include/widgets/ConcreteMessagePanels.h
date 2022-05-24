//
// Created by luke on 16-05-22.
//

#ifndef ROBOX_USER_INTERFACE_CONCRETEMESSAGEPANELS_H
#define ROBOX_USER_INTERFACE_CONCRETEMESSAGEPANELS_H

#include <utility>

#include "LoggerInterface.h"
#include "LogMessagePanel.h"

namespace Widgets
{
    class PanelError : public LogMessagePanel
    {
    public:
        explicit PanelError(wxWindow* parent, Logger::LogMessage message)
            : LogMessagePanel(parent, VerbosityType_e::ERROR), m_message(std::move(message))
        {
            Init();
        }
        ~PanelError() override = default;

    private:
        void Init();

    private:
        Logger::LogMessage m_message;
        const wxColour COLOR_BG_SW = wxColour(255, 0, 0);
        const wxColour COLOR_BG_FW = wxColour(100, 0, 0);
        const wxColour COLOR_FG_SW = wxColour(0, 0, 0);
        const wxColour COLOR_FG_FW = wxColour(255, 255, 255);
    };
    class PanelWarning : public LogMessagePanel
    {
    public:
        explicit PanelWarning(wxWindow* parent, Logger::LogMessage message)
            : LogMessagePanel(parent, VerbosityType_e::WARNING), m_message(std::move(message))
        {
            Init();
        }
        ~PanelWarning() override = default;

    private:
        void Init();

    private:
        Logger::LogMessage m_message;
        const wxColour COLOR_BG_SW = wxColour(255, 255, 0);
        const wxColour COLOR_BG_FW = wxColour(100, 100, 0);
        const wxColour COLOR_FG_SW = wxColour(0, 0, 0);
        const wxColour COLOR_FG_FW = wxColour(255, 255, 255);

    };
    class PanelInfo : public LogMessagePanel
    {
    public:
        explicit PanelInfo(wxWindow* parent, Logger::LogMessage message)
            : LogMessagePanel(parent, VerbosityType_e::INFO), m_message(std::move(message))
        {
            Init();
        }
        ~PanelInfo() override = default;

    private:
        void Init();

    private:
        Logger::LogMessage m_message;
        const wxColour COLOR_BG_SW = wxColour(0, 255, 0);
        const wxColour COLOR_BG_FW = wxColour(0, 100, 0);
        const wxColour COLOR_FG_SW = wxColour(0, 0, 0);
        const wxColour COLOR_FG_FW = wxColour(255, 255, 255);
    };
    class PanelDebug : public LogMessagePanel
    {
    public:
        explicit PanelDebug(wxWindow* parent, Logger::LogMessage message)
            : LogMessagePanel(parent, VerbosityType_e::DEBUG), m_message(std::move(message))
        {
            Init();
        }
        ~PanelDebug() override = default;

    private:
        void Init();

    private:
        Logger::LogMessage m_message;
        const wxColour COLOR_BG_SW = wxColour(255, 0, 255);
        const wxColour COLOR_BG_FW = wxColour(100, 0, 100);
        const wxColour COLOR_FG_SW = wxColour(0, 0, 0);
        const wxColour COLOR_FG_FW = wxColour(255, 255, 255);
    };
    class PanelTransition : public LogMessagePanel
    {
    public:
        explicit PanelTransition(wxWindow* parent, Logger::LogMessage message)
            : LogMessagePanel(parent, VerbosityType_e::INFO), m_message(std::move(message))
        {
            Init();
        }
        ~PanelTransition() override = default;

    private:
        void Init();

    private:
        Logger::LogMessage m_message;
        const wxColour COLOR_BG_SW = wxColour(0, 255, 0);
        const wxColour COLOR_BG_FW = wxColour(0, 100, 0);
        const wxColour COLOR_FG_SW = wxColour(0, 0, 0);
        const wxColour COLOR_FG_FW = wxColour(255, 255, 255);
    };


}// namespace Base

#endif//ROBOX_USER_INTERFACE_CONCRETEMESSAGEPANELS_H
