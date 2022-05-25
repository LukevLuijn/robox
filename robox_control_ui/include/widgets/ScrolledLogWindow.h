//
// Created by luke on 16-05-22.
//

#ifndef ROBOX_USER_INTERFACE_SCROLLEDLOGWINDOW_H
#define ROBOX_USER_INTERFACE_SCROLLEDLOGWINDOW_H

#include <vector>

#include "wx/scrolwin.h"

#include "ConcreteMessagePanels.h"
#include "msg_protocol.h"

namespace Widgets
{

    class ScrolledLogWindow : public wxScrolledWindow
    {
    public:
        explicit ScrolledLogWindow(wxWindow* parent);
        ~ScrolledLogWindow() override = default;

        /**
         * @brief Add message to the list.
         * 
         * @param message 
         * @param isReinstatement True if current call is a rewrite.
         */
        void AddMessage(const Logger::LogMessage& message, bool isReinstatement = false);
        /**
         * @brief Toggle the error message.
         * 
         */
        void ToggleError();
        /**
         * @brief Toggle the warning messages.
         * 
         */
        void ToggleWarning();
        /**
         * @brief Toggle the info messages.
         * 
         */
        void ToggleInfo();
        /**
         * @brief Toggle the debug messages.
         * 
         */
        void ToggleDebug();
        /**
         * @brief Toggle the external messages.
         * 
         */
        void ToggleExtern();
        /**
         * @brief Toggle the internal messages.
         * 
         */
        void ToggleIntern();
        /**
         * @brief Clear all messages from the panel.
         * 
         */
        void ClearPanel();

    private:
        void ToggleType(VerbosityType_e type);
        void ToggleSource(SourceType_e source);
        void Init();

    private:
         wxBoxSizer* m_sizer{};
         std::vector<Logger::LogMessage> m_backup;
         std::array<bool ,4> m_verbosity{};
         std::array<bool ,2> m_sources{};
    };
}// namespace Base


#endif//ROBOX_USER_INTERFACE_SCROLLEDLOGWINDOW_H
