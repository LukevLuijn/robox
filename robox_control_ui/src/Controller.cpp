// ======================================
// Filename:    Controller.cpp
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        17-05-2022
// ======================================

#include "Controller.h"
#include "Logger.h"
#include "RobotController.h"
#include "BKEDriver.h"


namespace Base
{
    namespace
    {
        std::function<void(const std::string&)> GetResponseCallback(Controller& instance)
        {
            return std::bind(&Controller::ResponseCallback, &instance, std::placeholders::_1);
        }
        std::function<void(const std::string&)> GetBKECallback(Controller& instance)
        {
            return std::bind(&Controller::BKECallback, &instance, std::placeholders::_1);
        }
    }
    Controller::Controller() : Frame::MainFrame(nullptr), m_controlActive(false)
    {
        SetNewControlFrame(Frame::FrameTypes_e::MANUAL_CTRL);
        Bind(UPDATE_EVENT, &Controller::OnUpdateEvent, this);
        Bind(LOG_EVENT, &Controller::OnLogEvent, this);
        Bind(BKE_EVENT, &Controller::OnBKEEvent, this);

        Bind(GAME_COMPLETE, &Controller::OnClickMenuStop, this);
    }
    void Controller::OnCloseWindow(wxCloseEvent& event)
    {
        if (wxMessageBox("Bye", "Confirm", wxOK, this))
        {
            Driver::RobotController::GetInstance().EStopRobot();

            // time for properly sending message.
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            m_buttonMenuConnect->SetValue(false);
            // Disconnect from serial
            wxCommandEvent toggleEvent(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED);
            OnClickMenuConnect(toggleEvent);
            // Kill window
            Destroy();
        }
        event.Skip();
    }
    void Controller::OnClickManual(wxCommandEvent& event)
    {
        SetNewControlFrame(Frame::FrameTypes_e::MANUAL_CTRL);
        INFO("New control method set", "Manual control");

        if (m_buttonMenuActivate->GetValue())
        {
            // if already homed, reset fields.
            builder.UpdateControlPanel(DataType_e::GET_RUNTIME);
            builder.UpdateControlPanel(DataType_e::GET_STATIC);
            builder.UpdateControlPanel(DataType_e::HOMING_COMPLETE);
        }

        event.Skip();
    }
    void Controller::OnClickKeyboard(wxCommandEvent& event)
    {
        // TODO
        WARNING("Work in progress");
        event.Skip();
    }
    void Controller::OnClickBKE(wxCommandEvent& event)
    {
        SetNewControlFrame(Frame::FrameTypes_e::BKE_CTRL);
        INFO("New control method set", "BKE control");

        if (m_buttonMenuActivate->GetValue())
        {
            // if already homed, reset fields.
            builder.UpdateControlPanel(DataType_e::GET_STATIC);
            builder.UpdateControlPanel(DataType_e::HOMING_COMPLETE);
        }

        event.Skip();
    }
    void Controller::OnClickSettings(wxCommandEvent& event)
    {
        // TODO
        WARNING("Work in progress");
        event.Skip();
    }
    void Controller::OnClickAbout(wxCommandEvent& event)
    {
        // TODO
        WARNING("Work in progress");
        event.Skip();
    }
    void Controller::OnClickMenuConnect(wxCommandEvent& event)
    {
        if (m_buttonMenuConnect->GetValue())
        {
            // TODO make dynamic
            Driver::RobotController::GetInstance().StartConnection("/dev/ttyACM0", 9600, GetResponseCallback(*this));
            Driver::BKEDriver::GetInstance().StartBKEConnection("/dev/ttyACM1", 9600, GetBKECallback(*this));

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            if (Driver::RobotController::GetInstance().IsConnected())
            {
                m_buttonMenuActivate->Enable();
                m_buttonMenuConnect->SetLabel("Disconnect");
            }
            else
            {
                m_buttonMenuConnect->SetValue(false);
            }
        }
        else
        {
            Driver::RobotController::GetInstance().StopRobot();
            std::this_thread::sleep_for(std::chrono::microseconds(500));

            Driver::RobotController::GetInstance().StopConnection();
            Driver::BKEDriver::GetInstance().StopBKEConnection();

            m_buttonMenuActivate->Disable();
            DeactivateControl();

            m_buttonMenuConnect->SetLabel("Connect");
        }
        event.Skip();
    }
    void Controller::OnClickMenuActivate(wxCommandEvent& event)
    {
        if (m_buttonMenuActivate->GetValue())
        {
            INFO("Starting robot");
            Driver::RobotController::GetInstance().StartRobot();
            ActivateControl();
        }
        else
        {
            INFO("Stopping robot");
            Driver::RobotController::GetInstance().StopRobot();
            DeactivateControl();
        }
        event.Skip();
    }
    void Controller::OnClickMenuRun(wxCommandEvent& event)
    {
        INFO("Moving to position");
        Driver::RobotController::GetInstance().RunRobot();
        event.Skip();
    }
    void Controller::OnClickMenuStop(wxCommandEvent& event)
    {
        INFO("Stopping robot");
        Driver::RobotController::GetInstance().StopRobot();
        DeactivateControl();
        event.Skip();
    }
    void Controller::OnClickMenuEStop(wxCommandEvent& event)
    {
        INFO("Emergency Stop send");
        Driver::RobotController::GetInstance().EStopRobot();
        DeactivateControl();
        event.Skip();
    }
    void Controller::OnUpdateEvent(wxCommandEvent& event)
    {
        auto type = static_cast<DataType_e>(event.GetString()[1] - '0');
        builder.UpdateControlPanel(type);

        if (type == DataType_e::HOMING_COMPLETE)
        {
            m_controlActive = true;
            m_placeholder->Enable();
        }
        event.Skip();
    }
    void Controller::OnBKEEvent(wxCommandEvent& event)
    {
        auto type = static_cast<BKEType_e>(event.GetString()[1] - '0');
        builder.UpdateControlPanel(type);
        INFO("ON BKE EVENT");
        event.Skip();
    }
    void Controller::OnLogEvent(wxCommandEvent& event)
    {
        m_logPanel->WriteMessage();
        Layout();
        event.Skip();
    }
    void Controller::OnGameComplete(wxCommandEvent& event)
    {
        WARNING("GAME COMPLETE");
        Driver::RobotController::GetInstance().StopRobot();
        DeactivateControl();
        m_buttonMenuActivate->SetValue(false);
    }
    void Controller::ResponseCallback(const std::string& response)
    {
        wxCommandEvent event(UPDATE_EVENT);
        event.SetString(response);
        wxPostEvent(this, event);
    }
    void Controller::BKECallback(const std::string& message)
    {
        wxCommandEvent event(BKE_EVENT);
        event.SetString(message);
        wxPostEvent(this, event);
    }
    void Controller::DeactivateRobot()
    {
        INFO("Deactivating robot.");

        wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED);
        OnClickMenuStop(event);
    }
    void Controller::ActivateRobot()
    {
        INFO("Activating robot.");

        m_buttonMenuActivate->SetValue(true);

        wxCommandEvent event(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED);
        OnClickMenuActivate(event);
    }
    void Controller::SetNewControlFrame(Frame::FrameTypes_e type)
    {
        if (!m_placeholderSizer->IsEmpty())
        {
            m_placeholderSizer->DeleteWindows();
        }

        builder.SetCurrentFrame(m_placeholder, type);
        m_placeholderSizer->Add(builder.GetCurrentPanel(), 1, wxEXPAND | wxALL, 5);
        m_placeholderSizer->Layout();
        m_placeholderSizer->FitInside(m_placeholderSizer->GetContainingWindow());
        m_placeholderSizer->GetContainingWindow()->Update();

        for (wxButton* button : m_menuButtons)
        {
            button->Enable();
        }
        m_menuButtons[(size_t) type]->Disable();

        if (m_controlActive)
        {
            m_placeholder->Enable();
        }
        else
        {
            m_placeholder->Disable();
        }
    }
    void Controller::ActivateControl()
    {
        m_buttonMenuRun->Enable();
//        m_placeholder->Enable();
        m_buttonMenuStop->Enable();
        m_buttonMenuEStop->Enable();
        m_buttonMenuActivate->SetLabel("Deactivate");

        builder.ResetControlPanel();
    }
    void Controller::DeactivateControl()
    {
        m_buttonMenuActivate->SetValue(false);

        m_buttonMenuRun->Disable();
        m_placeholder->Disable();
        m_buttonMenuStop->Disable();
        m_buttonMenuEStop->Disable();
        m_buttonMenuActivate->SetLabel("Activate");

        builder.ResetControlPanel();

        m_controlActive = false;
    }
    void Controller::PostLogEvent()
    {
       wxCommandEvent commandEvent(LOG_EVENT);
       wxPostEvent(this, commandEvent);
    }
}// namespace Base