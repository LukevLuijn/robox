// ======================================
// Filename:    MainFrame.h
// Project:     ROBOX Control UI
// Subsystem:   Main Application
// Author:      Luke van Luijn
// Data:        17-05-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_MAIN_FRAME_H
#define ROBOX_CONTROL_UI_MAIN_FRAME_H

#include "wx/wx.h"
#include "wx/statline.h"
#include "wx/tglbtn.h"

#include <array>

#include "FrameInterface.h"
#include "LogPanel.h"

namespace Frame
{
    class MainFrame : public wxFrame
    {
    public:
        explicit MainFrame(wxWindow* parent);
        ~MainFrame() override;

    protected:
        virtual void OnCloseWindow( wxCloseEvent& event );
        virtual void OnClickManual(wxCommandEvent& event);
        virtual void OnClickKeyboard(wxCommandEvent& event);
        virtual void OnClickBKE(wxCommandEvent& event);
        virtual void OnClickSettings(wxCommandEvent& event);
        virtual void OnClickAbout(wxCommandEvent& event);
        virtual void OnClickMenuConnect(wxCommandEvent& event);
        virtual void OnClickMenuActivate(wxCommandEvent& event);
        virtual void OnClickMenuRun(wxCommandEvent& event);
        virtual void OnClickMenuStop(wxCommandEvent& event);
        virtual void OnClickMenuEStop(wxCommandEvent& event);

    private:
        void InitializeFrame();
        void ConnectEvents();
        void DisconnectEvents();

    protected:

        std::array<wxButton*, 5> m_menuButtons;

        wxPanel* m_menuPanel;
        Widgets::LogPanel* m_logPanel;

        wxToggleButton* m_buttonMenuConnect;
        wxToggleButton* m_buttonMenuActivate;
        wxBitmapButton* m_buttonMenuStop;
        wxBitmapButton* m_buttonMenuEStop;
        wxButton* m_buttonMenuRun;

        wxStaticLine* m_menuLine0;
        wxStaticLine* m_menuLine1;
        wxStaticLine* m_menuLine2;

        wxPanel* m_placeholder;
        wxBoxSizer* m_placeholderSizer;
    };
}// namespace Frame

#endif//ROBOX_CONTROL_UI_MAIN_FRAME_H
