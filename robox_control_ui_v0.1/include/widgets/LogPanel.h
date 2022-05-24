//
// Created by luke on 16-05-22.
//

#ifndef ROBOX_USER_INTERFACE_LOGPANEL_H
#define ROBOX_USER_INTERFACE_LOGPANEL_H

#include "ScrolledLogWindow.h"

#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/tglbtn.h>

#include <queue>

namespace Widgets
{


    class LogPanel : public wxPanel
    {
    public:
        explicit LogPanel(wxWindow* parent);
        ~LogPanel() override;
        /**
         * @brief Trigger a log message write.
         * 
         */
        void WriteMessage();

    protected:
        virtual void OnToggleApplication(wxCommandEvent& event);
        virtual void OnToggleFirmware(wxCommandEvent& event);
        virtual void OnClickClear(wxCommandEvent& event);
        virtual void OnToggleVerbosityError(wxCommandEvent& event);
        virtual void OnToggleVerbosityWarning(wxCommandEvent& event);
        virtual void OnToggleVerbosityInfo(wxCommandEvent& event);
        virtual void OnToggleVerbosityDebug(wxCommandEvent& event);
    private:
        void InitializeFrame();
        void ConnectEvents();
        void DisconnectEvents();

    public:
        static std::queue<Logger::LogMessage> m_messageQueue;

    private:
        const wxSize IMAGE_SIZE = wxSize(32,32);

        wxPanel* m_panel2{};
        wxPanel* m_panel49{};
        wxPanel* m_verbosityPanel{};

        wxToggleButton* m_buttonApplication{};
        wxToggleButton* m_buttonFirmware{};
        wxButton* m_buttonClear{};

        wxBitmapToggleButton* m_buttonVerbosityError{};
        wxBitmapToggleButton* m_buttonVerbosityWarning{};
        wxBitmapToggleButton* m_buttonVerbosityInfo{};
        wxBitmapToggleButton* m_buttonVerbosityDebug{};

        wxStaticLine* m_staticLine3{};
        wxStaticLine* m_staticLine4{};

        ScrolledLogWindow* m_logWindow{};
    };
}// namespace Widgets

#endif//ROBOX_USER_INTERFACE_LOGPANEL_H
