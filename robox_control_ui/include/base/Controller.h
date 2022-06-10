// ======================================
// Filename:    Controller.h
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        17-05-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_CONTROLLER_H
#define ROBOX_CONTROL_UI_CONTROLLER_H

#include "FrameBuilder.h"
#include "MainFrame.h"

namespace Base
{
    wxDEFINE_EVENT(UPDATE_EVENT, wxCommandEvent);
    wxDEFINE_EVENT(BKE_EVENT, wxCommandEvent);
    wxDEFINE_EVENT(LOG_EVENT, wxCommandEvent);

    class Controller : public Frame::MainFrame
    {
    public:
        Controller();
        ~Controller() override = default;
        /**
         * @brief Post Log event in main thread.
         * 
         * @note before calling this method the message should be added to the message queue in @see LogPanel.h
         * 
         */
        void PostLogEvent();
        /**
         * @brief Callback for handling incoming responses.
         * 
         * @param response Incoming response.
         */
        void ResponseCallback(const std::string& response);
        /**
         * @brief Callback for handeling incoming BKE messages.
         *
         * @param Incomming BKE message.
         */
        void BKECallback(const std::string& message);

    private:
        void OnCloseWindow(wxCloseEvent& event) override;
        void OnClickManual(wxCommandEvent& event) override;
        void OnClickKeyboard(wxCommandEvent& event) override;
        void OnClickBKE(wxCommandEvent& event) override;
        void OnClickSettings(wxCommandEvent& event) override;
        void OnClickAbout(wxCommandEvent& event) override;
        void OnClickMenuConnect(wxCommandEvent& event) override;
        void OnClickMenuActivate(wxCommandEvent& event) override;
        void OnClickMenuRun(wxCommandEvent& event) override;
        void OnClickMenuStop(wxCommandEvent& event) override;
        void OnClickMenuEStop(wxCommandEvent& event) override;

        void OnUpdateEvent(wxCommandEvent& event);
        void OnBKEEvent(wxCommandEvent& event);
        void OnLogEvent(wxCommandEvent& event);


    private:
        void SetNewControlFrame(Frame::FrameTypes_e type);
        void ActivateControl();
        void DeactivateControl();

    private:
        FrameBuilder builder;
        bool m_controlActive;
    };
}// namespace Base

#endif//ROBOX_CONTROL_UI_CONTROLLER_H
