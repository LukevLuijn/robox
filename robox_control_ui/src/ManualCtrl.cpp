//
// Created by luke on 21-05-22.
//

#include "ManualCtrl.h"

#include <limits>

#include "../include/logger/Logger.h"
#include "RobotController.h"
#include "String.h"

namespace Frame
{
    ManualCtrl::ManualCtrl(wxWindow* parent) : IFrame(parent), m_updateFromDrawPanel(false)
    {
        InitializeFrame();
        ConnectEvents();
    }
    ManualCtrl::~ManualCtrl()
    {
        DisconnectEvents();
    }
    wxPanel* ManualCtrl::GetPanel()
    {
        return this;
    }
    void ManualCtrl::UpdateData(DataType_e responseType)
    {
        switch (responseType)
        {
            case DataType_e::HOMING_COMPLETE:
                INFO("Homing sequence complete");
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                SyncFields();
                Enable();
                break;
            case DataType_e::GET_RUNTIME:
                m_drawPanel->UpdateData();
                break;
            case DataType_e::GET_STATIC:
                UpdateFields();
                break;
            case DataType_e::GET_DYNAMIC:
            case DataType_e::SET_SPEED_ACCEL:
            case DataType_e::SET_VERBOSITY:
            case DataType_e::SET_FRAME_OFFSET:
            case DataType_e::SET_GRIP_OFFSET:
            case DataType_e::SET_IDLE_POSITIONS:
            default:
                break;
        }
        Refresh();
        Update();
    }
    void ManualCtrl::ResetPanel()
    {
        // TODO

        m_toggleAuto->SetValue(false);
        wxCommandEvent eventToggle(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED);
        OnToggleAuto(eventToggle);

        Refresh();
        Update();
    }
    void ManualCtrl::UpdateFields()
    {
        m_minMaxValues[(size_t)Fields_e::FIELD_SPEED].first = 0;
        m_minMaxValues[(size_t)Fields_e::FIELD_SPEED].second = 100;
        m_minMaxValues[(size_t)Fields_e::FIELD_ACCEL].first = 0;
        m_minMaxValues[(size_t)Fields_e::FIELD_ACCEL].second = 100;

        m_sliderSpeed->SetMin(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_SPEED].first * 100));
        m_sliderSpeed->SetMax(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_SPEED].second * 100));

        m_sliderAccel->SetMin(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_ACCEL].first * 100));
        m_sliderAccel->SetMax(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_ACCEL].second * 100));

        m_minMaxValues[(size_t) Fields_e::FIELD_Z].first =
                static_cast<float>(Driver::RobotDriver::m_dataManager.m_segment00.m_minPosition);
        m_minMaxValues[(size_t) Fields_e::FIELD_Z].second =
                static_cast<float>(Driver::RobotDriver::m_dataManager.m_actualZeroPosSeg00);
        m_minMaxValues[(size_t) Fields_e::FIELD_A1].first =
                static_cast<float>(Driver::RobotDriver::m_dataManager.m_segment01.m_minPosition);
        m_minMaxValues[(size_t) Fields_e::FIELD_A1].second =
                static_cast<float>(Driver::RobotDriver::m_dataManager.m_segment01.m_maxPosition);
        m_minMaxValues[(size_t) Fields_e::FIELD_A2].first =
                static_cast<float>(Driver::RobotDriver::m_dataManager.m_segment02.m_minPosition);
        m_minMaxValues[(size_t) Fields_e::FIELD_A2].second =
                static_cast<float>(Driver::RobotDriver::m_dataManager.m_segment02.m_maxPosition);
        m_minMaxValues[(size_t) Fields_e::FIELD_A3].first =
                static_cast<float>(Driver::RobotDriver::m_dataManager.m_segment03.m_minPosition);
        m_minMaxValues[(size_t) Fields_e::FIELD_A3].second =
                static_cast<float>(Driver::RobotDriver::m_dataManager.m_segment03.m_maxPosition);

        m_sliderZ->SetMin(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_Z].first * 100));
        m_sliderZ->SetMax(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_Z].second * 100));
        m_sliderA1->SetMin(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_A1].first * 100));
        m_sliderA1->SetMax(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_A1].second * 100));
        m_sliderA2->SetMin(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_A2].first * 100));
        m_sliderA2->SetMax(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_A2].second * 100));
        m_sliderA3->SetMin(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_A3].first * 100));
        m_sliderA3->SetMax(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_A3].second * 100));

        Refresh();
        Update();

        INFO("ManualCtrl fields updated");
    }
    void ManualCtrl::SyncFields()
    {
        const float startSpeed = 100*100;
        const float startAccel = 100*100;
        const float currentPositionSeg00 = Driver::RobotDriver::m_dataManager.m_segment00.m_currentPosition;
        const float currentPositionSeg01 = Driver::RobotDriver::m_dataManager.m_segment01.m_currentPosition;
        const float currentPositionSeg02 = Driver::RobotDriver::m_dataManager.m_segment02.m_currentPosition;
        const float currentPositionSeg03 = Driver::RobotDriver::m_dataManager.m_segment03.m_currentPosition;

        m_textCtrlSpeed->SetValue(Utils::String::ToString(startSpeed / 100, 2));
        m_textCtrlAccel->SetValue(Utils::String::ToString(startAccel / 100, 2));
        m_textCtrlZ->SetValue(Utils::String::ToString(currentPositionSeg00, 2));
        m_textCtrlA1->SetValue(Utils::String::ToString(currentPositionSeg01, 2));
        m_textCtrlA2->SetValue(Utils::String::ToString(currentPositionSeg02, 2));
        m_textCtrlA3->SetValue(Utils::String::ToString(currentPositionSeg03, 2));

        m_sliderSpeed->SetValue(static_cast<int32_t>(startSpeed));
        m_sliderAccel->SetValue(static_cast<int32_t>(startAccel));
        m_sliderZ->SetValue(static_cast<int32_t>(currentPositionSeg00 * 100));
        m_sliderA1->SetValue(static_cast<int32_t>(currentPositionSeg01 * 100));
        m_sliderA2->SetValue(static_cast<int32_t>(currentPositionSeg02 * 100));
        m_sliderA3->SetValue(static_cast<int32_t>(currentPositionSeg03 * 100));

        Driver::RobotController::GetInstance().SetNewSpeed(startSpeed/100);
        Driver::RobotController::GetInstance().SetNewAccel(startAccel/100);

        Driver::RobotController::GetInstance().SetNewPositionA0(
                Driver::RobotDriver::m_dataManager.m_segment00.m_currentPosition);
        Driver::RobotController::GetInstance().SetNewPositionA1(
                Driver::RobotDriver::m_dataManager.m_segment01.m_currentPosition);
        Driver::RobotController::GetInstance().SetNewPositionA2(
                Driver::RobotDriver::m_dataManager.m_segment02.m_currentPosition);
        Driver::RobotController::GetInstance().SetNewPositionA3(
                Driver::RobotDriver::m_dataManager.m_segment03.m_currentPosition);

        m_sliderAccel->Refresh();
        m_sliderSpeed->Refresh();
        m_sliderZ->Refresh();
        m_sliderA1->Refresh();
        m_sliderA2->Refresh();
        m_sliderA3->Refresh();

        Refresh();
        Update();

        INFO("ManualCtrl fields synchronized");
    }
    bool ManualCtrl::OnTextEnter(wxTextCtrl* textCtrl, wxSlider* slider, float& value, Fields_e field)
    {
        float min = m_minMaxValues[(size_t) field].first;
        float max = m_minMaxValues[(size_t) field].second;
        std::string textValue = textCtrl->GetValue().ToStdString();

        if (!Utils::String::ValidateNumber(value, textValue, min, max))
        {
            textCtrl->SetForegroundColour(wxColour(255, 0, 0));
        }
        else
        {
            textCtrl->SetForegroundColour(wxColour(0, 0, 0));
            textCtrl->SetValue(Utils::String::ToString(value));
            slider->SetValue(static_cast<int32_t>(value * 100));
            INFO("New position set");
            return true;
        }
        ERROR("Could not translate input");
        return false;
    }
    void ManualCtrl::OnSliderMove(wxTextCtrl* textCtrl, wxSlider* slider)
    {
        auto value = static_cast<float>(slider->GetValue()) / 100;
        textCtrl->SetValue(Utils::String::ToString(value, 2));
    }
    void ManualCtrl::UpdateDrawPanel()
    {
        std::array<double, 2> angles = m_drawPanel->GetAngles();
        if (Utils::String::ToString(angles[0], 2) != m_textCtrlSpeed->GetValue().ToStdString() ||
            Utils::String::ToString(angles[1], 2) != m_textCtrlAccel->GetValue().ToStdString())
        {
            double speed = std::numeric_limits<double>::max();
            double accel = std::numeric_limits<double>::max();

            try
            {
                speed = std::stod(m_textCtrlSpeed->GetValue().ToStdString());
                accel = std::stod(m_textCtrlAccel->GetValue().ToStdString());
            }
            catch (...)
            {
                // ignore..
            }

            if (speed != std::numeric_limits<double>::max() && accel != std::numeric_limits<double>::max())
            {
                std::array<double, 2> newAngles = {speed, accel};
                m_drawPanel->SetAngles(newAngles);
            }
        }
    }
    void ManualCtrl::OnTextEnterSpeed(wxCommandEvent& event)
    {
        float value = 0.f;
        if (OnTextEnter(m_textCtrlSpeed, m_sliderSpeed, value, Fields_e::FIELD_SPEED))
        {
            Driver::RobotController::GetInstance().SetNewSpeed(value);
        }
        event.Skip();
    }
    void ManualCtrl::OnTextEnterAccel(wxCommandEvent& event)
    {
        float value = 0.f;
        if (OnTextEnter(m_textCtrlAccel, m_sliderAccel, value, Fields_e::FIELD_ACCEL))
        {
            Driver::RobotController::GetInstance().SetNewAccel(value);
        }
        event.Skip();
    }
    void ManualCtrl::OnTextEnterZ(wxCommandEvent& event)
    {
        float value = 0.f;
        if (OnTextEnter(m_textCtrlZ, m_sliderZ, value, Fields_e::FIELD_Z))
        {
            Driver::RobotController::GetInstance().SetNewPositionA0(value);
        }
        event.Skip();
    }
    void ManualCtrl::OnTextEnterA1(wxCommandEvent& event)
    {
        float value = 0.f;
        if (OnTextEnter(m_textCtrlA1, m_sliderA1, value, Fields_e::FIELD_A1))
        {
            if (!m_updateFromDrawPanel)
            {
                UpdateDrawPanel();
            }

            if (Driver::RobotController::GetInstance().GetDirectSend())
            {
                float valueA1 = value;
                float valueA2 = static_cast<float>(m_sliderA2->GetValue()) / 100;
                Driver::RobotController::GetInstance().SetNewPositionA12(valueA1, valueA2);
            }
            else
            {
                Driver::RobotController::GetInstance().SetNewPositionA1(value);
            }
        }
        event.Skip();
    }
    void ManualCtrl::OnTextEnterA2(wxCommandEvent& event)
    {
        float value = 0.f;
        if (OnTextEnter(m_textCtrlA2, m_sliderA2, value, Fields_e::FIELD_A2))
        {
            if (!m_updateFromDrawPanel)
            {
                UpdateDrawPanel();
            }
            if (Driver::RobotController::GetInstance().GetDirectSend())
            {
                float valueA1 = static_cast<float>(m_sliderA1->GetValue()) / 100;
                float valueA2 = value;
                Driver::RobotController::GetInstance().SetNewPositionA12(valueA1, valueA2);
            }
            else
            {
                Driver::RobotController::GetInstance().SetNewPositionA2(value);
            }
        }
        event.Skip();
    }
    void ManualCtrl::OnTextEnterA3(wxCommandEvent& event)
    {
        float value = 0.f;
        if (OnTextEnter(m_textCtrlA3, m_sliderA3, value, Fields_e::FIELD_A3))
        {
            Driver::RobotController::GetInstance().SetNewPositionA3(value);
        }
        event.Skip();
    }
    void ManualCtrl::OnKillFocusSpeed(wxFocusEvent& event)
    {
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
        OnTextEnterSpeed(commandEvent);
        event.Skip();
    }
    void ManualCtrl::OnKillFocusAccel(wxFocusEvent& event)
    {
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
        OnTextEnterAccel(commandEvent);
        event.Skip();
    }
    void ManualCtrl::OnKillFocusZ(wxFocusEvent& event)
    {
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
        OnTextEnterZ(commandEvent);
        event.Skip();
    }
    void ManualCtrl::OnKillFocusA1(wxFocusEvent& event)
    {
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
        OnTextEnterA1(commandEvent);
        event.Skip();
    }
    void ManualCtrl::OnKillFocusA2(wxFocusEvent& event)
    {
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
        OnTextEnterA2(commandEvent);
        event.Skip();
    }
    void ManualCtrl::OnKillFocusA3(wxFocusEvent& event)
    {
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
        OnTextEnterA3(commandEvent);
        event.Skip();
    }
    void ManualCtrl::OnSliderReleaseSpeed(wxScrollEvent& event)
    {
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
        OnTextEnterSpeed(commandEvent);
        event.Skip();
    }
    void ManualCtrl::OnSliderReleaseAccel(wxScrollEvent& event)
    {
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
        OnTextEnterAccel(commandEvent);
        event.Skip();
    }
    void ManualCtrl::OnSliderReleaseZ(wxScrollEvent& event)
    {
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
        OnTextEnterZ(commandEvent);
        event.Skip();
    }
    void ManualCtrl::OnSliderReleaseA1(wxScrollEvent& event)
    {
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
        OnTextEnterA1(commandEvent);
        event.Skip();
    }
    void ManualCtrl::OnSliderReleaseA2(wxScrollEvent& event)
    {
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
        OnTextEnterA2(commandEvent);
        event.Skip();
    }
    void ManualCtrl::OnSliderReleaseA3(wxScrollEvent& event)
    {
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
        OnTextEnterA3(commandEvent);
        event.Skip();
    }
    void ManualCtrl::OnSliderSpeed(wxCommandEvent& event)
    {
        OnSliderMove(m_textCtrlSpeed, m_sliderSpeed);
        event.Skip();
    }
    void ManualCtrl::OnSliderAccel(wxCommandEvent& event)
    {
        OnSliderMove(m_textCtrlAccel, m_sliderAccel);
        event.Skip();
    }
    void ManualCtrl::OnSliderZ(wxCommandEvent& event)
    {
        OnSliderMove(m_textCtrlZ, m_sliderZ);
        event.Skip();
    }
    void ManualCtrl::OnSliderA1(wxCommandEvent& event)
    {
        OnSliderMove(m_textCtrlA1, m_sliderA1);

        std::array<double, 2> angles = {
                static_cast<double>(m_sliderA1->GetValue()) / 100,
                static_cast<double>(m_sliderA2->GetValue()) / 100,
        };
        m_drawPanel->SetAngles(angles);
        event.Skip();
    }
    void ManualCtrl::OnSliderA2(wxCommandEvent& event)
    {
        OnSliderMove(m_textCtrlA2, m_sliderA2);

        std::array<double, 2> angles = {
                static_cast<double>(m_sliderA1->GetValue()) / 100,
                static_cast<double>(m_sliderA2->GetValue()) / 100,
        };
        m_drawPanel->SetAngles(angles);
        event.Skip();
    }
    void ManualCtrl::OnSliderA3(wxCommandEvent& event)
    {
        OnSliderMove(m_textCtrlA3, m_sliderA3);
        event.Skip();
    }
    void ManualCtrl::OnToggleGrip01(wxCommandEvent& event)
    {
        uint8_t gripperValue = (m_toggleGrip01->GetValue()) ? 255 : 0;
        Driver::RobotController::GetInstance().SetGripperValue01(gripperValue);
        INFO("Gripper 01 value set", std::to_string(gripperValue));
        event.Skip();
    }
    void ManualCtrl::OnToggleGrip02(wxCommandEvent& event)
    {
        uint8_t gripperValue = (m_toggleGrip02->GetValue()) ? 255 : 0;
        Driver::RobotController::GetInstance().SetGripperValue02(gripperValue);
        INFO("Gripper 02 value set", std::to_string(gripperValue));
        event.Skip();
    }
    void ManualCtrl::OnToggleAuto(wxCommandEvent& event)
    {
        SyncFields();
        Driver::RobotController::GetInstance().SetDirectSend(m_toggleAuto->GetValue());

        INFO("New control mode set", (m_toggleAuto->GetValue()) ? "AUTOMATIC" : "MANUAL");
        event.Skip();
    }
    void ManualCtrl::OnMouseRelease(wxMouseEvent& event)
    {
        if (event.GetId() == 0)
        {
            INFO("Cartesian position set");

            std::array<double, 2> angles = m_drawPanel->GetAngles();

            m_textCtrlA1->SetValue(Utils::String::ToString(angles[0], 2));
            m_textCtrlA2->SetValue(Utils::String::ToString(angles[1], 2));

            m_updateFromDrawPanel = true;
            wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
            OnTextEnterA1(commandEvent);
            OnTextEnterA2(commandEvent);
            m_updateFromDrawPanel = false;

            m_sliderA1->Refresh();
            m_sliderA2->Refresh();

            Refresh();
            Update();
        }
        event.Skip();

    }
    void ManualCtrl::OnMouseMotion(wxMouseEvent& event)
    {
        if (event.GetId() == 0)
        {
            std::array<double, 2> angles = m_drawPanel->GetAngles();

            m_sliderA1->SetValue(static_cast<int32_t>(angles[0]) * 100);
            m_sliderA2->SetValue(static_cast<int32_t>(angles[1]) * 100);

            OnSliderMove(m_textCtrlA1, m_sliderA1);
            OnSliderMove(m_textCtrlA2, m_sliderA2);

            m_sliderA1->Refresh();
            m_sliderA2->Refresh();

            Refresh();
            Update();
        }
        event.Skip();
    }
    void ManualCtrl::OnTextMaxLenSpeed(wxCommandEvent& event)
    {
        WARNING("Max amount of characters reached.", "6");
        event.Skip();
    }
    void ManualCtrl::OnTextMaxLenAccel(wxCommandEvent& event)
    {
        WARNING("Max amount of characters reached.", "6");
        event.Skip();
    }
    void ManualCtrl::OnTextMaxLenZ(wxCommandEvent& event)
    {
        WARNING("Max amount of characters reached.", "7");
        event.Skip();
    }
    void ManualCtrl::OnTextMaxLenA1(wxCommandEvent& event)
    {
        WARNING("Max amount of characters reached.", "7");
        event.Skip();
    }
    void ManualCtrl::OnTextMaxLenA2(wxCommandEvent& event)
    {
        WARNING("Max amount of characters reached.", "7");
        event.Skip();
    }
    void ManualCtrl::OnTextMaxLenA3(wxCommandEvent& event)
    {
        WARNING("Max amount of characters reached.", "7");
        event.Skip();
    }
    void ManualCtrl::InitializeFrame()
    {
        this->SetSizeHints(wxDefaultSize, wxDefaultSize);

        wxBoxSizer* bSizer12;
        bSizer12 = new wxBoxSizer(wxVERTICAL);

        m_configPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxFlexGridSizer* fgSizer3;
        fgSizer3 = new wxFlexGridSizer(2, 4, 0, 0);
        fgSizer3->AddGrowableCol(3);
        fgSizer3->SetFlexibleDirection(wxBOTH);
        fgSizer3->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

        m_textSpeed = new wxStaticText(m_configPanel, wxID_ANY, wxT("Speed:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textSpeed->Wrap(-1);
        fgSizer3->Add(m_textSpeed, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

        m_textCtrlSpeed = new wxTextCtrl(m_configPanel,
                                         wxID_ANY,
                                         wxEmptyString,
                                         wxDefaultPosition,
                                         wxDefaultSize,
                                         wxTE_PROCESS_ENTER);
        fgSizer3->Add(m_textCtrlSpeed, 0, wxALL, 5);

        m_iconSpeed = new wxStaticText(m_configPanel, wxID_ANY, wxT("%"), wxDefaultPosition, wxDefaultSize, 0);
        m_iconSpeed->Wrap(-1);
        fgSizer3->Add(m_iconSpeed, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM | wxRIGHT, 5);

        m_sliderSpeed = new wxSlider(m_configPanel,
                                     wxID_ANY,
                                     5000,
                                     0,
                                     10000,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxSL_HORIZONTAL);
        fgSizer3->Add(m_sliderSpeed, 0, wxALL | wxEXPAND, 5);

        m_textAccel = new wxStaticText(m_configPanel, wxID_ANY, wxT("Accel:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textAccel->Wrap(-1);
        fgSizer3->Add(m_textAccel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

        m_textCtrlAccel = new wxTextCtrl(m_configPanel,
                                         wxID_ANY,
                                         wxEmptyString,
                                         wxDefaultPosition,
                                         wxDefaultSize,
                                         wxTE_PROCESS_ENTER);
        fgSizer3->Add(m_textCtrlAccel, 0, wxALL, 5);

        m_iconAccel = new wxStaticText(m_configPanel, wxID_ANY, wxT("%"), wxDefaultPosition, wxDefaultSize, 0);
        m_iconAccel->Wrap(-1);
        fgSizer3->Add(m_iconAccel, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM | wxRIGHT, 5);

        m_sliderAccel = new wxSlider(m_configPanel,
                                     wxID_ANY,
                                     5000,
                                     0,
                                     10000,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxSL_HORIZONTAL);
        fgSizer3->Add(m_sliderAccel, 0, wxALL | wxEXPAND, 5);


        m_configPanel->SetSizer(fgSizer3);
        m_configPanel->Layout();
        fgSizer3->Fit(m_configPanel);
        bSizer12->Add(m_configPanel, 0, wxEXPAND, 5);

        m_line0 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        bSizer12->Add(m_line0, 0, wxEXPAND | wxALL, 5);

        m_togglePanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxBoxSizer* bSizer3;
        bSizer3 = new wxBoxSizer(wxHORIZONTAL);

        m_toggleGrip01 =
                new wxToggleButton(m_togglePanel, wxID_ANY, wxT("Gripper 01"), wxDefaultPosition, wxDefaultSize, 0);
        bSizer3->Add(m_toggleGrip01, 0, wxALL, 5);

        m_toggleGrip02 =
                new wxToggleButton(m_togglePanel, wxID_ANY, wxT("Gripper 02"), wxDefaultPosition, wxDefaultSize, 0);
        bSizer3->Add(m_toggleGrip02, 0, wxALL, 5);

        m_line1 = new wxStaticLine(m_togglePanel,
                                   wxID_ANY,
                                   wxDefaultPosition,
                                   wxDefaultSize,
                                   wxLI_HORIZONTAL | wxLI_VERTICAL);
        bSizer3->Add(m_line1, 0, wxEXPAND | wxALL, 5);

        m_toggleAuto =
                new wxToggleButton(m_togglePanel, wxID_ANY, wxT("Automatic run"), wxDefaultPosition, wxDefaultSize, 0);
        bSizer3->Add(m_toggleAuto, 1, wxALL, 5);


        m_togglePanel->SetSizer(bSizer3);
        m_togglePanel->Layout();
        bSizer3->Fit(m_togglePanel);
        bSizer12->Add(m_togglePanel, 0, wxEXPAND, 5);

        m_line2 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        bSizer12->Add(m_line2, 0, wxEXPAND | wxALL, 5);

        m_positionPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxFlexGridSizer* fgSizer6;
        fgSizer6 = new wxFlexGridSizer(4, 4, 0, 0);
        fgSizer6->AddGrowableCol(3);
        fgSizer6->SetFlexibleDirection(wxBOTH);
        fgSizer6->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

        m_textZ = new wxStaticText(m_positionPanel, wxID_ANY, wxT("Z:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textZ->Wrap(-1);
        fgSizer6->Add(m_textZ, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

        m_textCtrlZ = new wxTextCtrl(m_positionPanel,
                                     wxID_ANY,
                                     wxEmptyString,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxTE_PROCESS_ENTER);
        fgSizer6->Add(m_textCtrlZ, 0, wxALL, 5);

        m_iconZ = new wxStaticText(m_positionPanel, wxID_ANY, wxT("mm"), wxDefaultPosition, wxDefaultSize, 0);
        m_iconZ->Wrap(-1);
        fgSizer6->Add(m_iconZ, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM | wxRIGHT, 5);

        m_sliderZ =
                new wxSlider(m_positionPanel, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
        fgSizer6->Add(m_sliderZ, 0, wxALL | wxEXPAND, 5);

        m_textA1 = new wxStaticText(m_positionPanel, wxID_ANY, wxT("A1:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textA1->Wrap(-1);
        fgSizer6->Add(m_textA1, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

        m_textCtrlA1 = new wxTextCtrl(m_positionPanel,
                                      wxID_ANY,
                                      wxEmptyString,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxTE_PROCESS_ENTER);
        fgSizer6->Add(m_textCtrlA1, 0, wxALL, 5);

        m_iconA1 = new wxStaticText(m_positionPanel, wxID_ANY, wxT("°"), wxDefaultPosition, wxDefaultSize, 0);
        m_iconA1->Wrap(-1);
        fgSizer6->Add(m_iconA1, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM | wxRIGHT, 5);

        m_sliderA1 =
                new wxSlider(m_positionPanel, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
        fgSizer6->Add(m_sliderA1, 0, wxALL | wxEXPAND, 5);

        m_textA2 = new wxStaticText(m_positionPanel, wxID_ANY, wxT("A2:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textA2->Wrap(-1);
        fgSizer6->Add(m_textA2, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

        m_textCtrlA2 = new wxTextCtrl(m_positionPanel,
                                      wxID_ANY,
                                      wxEmptyString,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxTE_PROCESS_ENTER);
        fgSizer6->Add(m_textCtrlA2, 0, wxALL, 5);

        m_iconA2 = new wxStaticText(m_positionPanel, wxID_ANY, wxT("°"), wxDefaultPosition, wxDefaultSize, 0);
        m_iconA2->Wrap(-1);
        fgSizer6->Add(m_iconA2, 0, wxTOP | wxBOTTOM | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);

        m_sliderA2 =
                new wxSlider(m_positionPanel, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
        fgSizer6->Add(m_sliderA2, 0, wxALL | wxEXPAND, 5);

        m_textA3 = new wxStaticText(m_positionPanel, wxID_ANY, wxT("A3:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textA3->Wrap(-1);
        fgSizer6->Add(m_textA3, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

        m_textCtrlA3 = new wxTextCtrl(m_positionPanel,
                                      wxID_ANY,
                                      wxEmptyString,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxTE_PROCESS_ENTER);
        fgSizer6->Add(m_textCtrlA3, 0, wxALL, 5);

        m_iconA3 = new wxStaticText(m_positionPanel, wxID_ANY, wxT("°"), wxDefaultPosition, wxDefaultSize, 0);
        m_iconA3->Wrap(-1);
        fgSizer6->Add(m_iconA3, 0, wxTOP | wxBOTTOM | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);

        m_sliderA3 =
                new wxSlider(m_positionPanel, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
        fgSizer6->Add(m_sliderA3, 0, wxALL | wxEXPAND, 5);


        m_positionPanel->SetSizer(fgSizer6);
        m_positionPanel->Layout();
        fgSizer6->Fit(m_positionPanel);
        bSizer12->Add(m_positionPanel, 0, wxEXPAND, 5);

        int32_t lengths[] = {240, 150};
        m_drawPanel = new RobotDrawPanel(this, lengths);

        bSizer12->Add(m_drawPanel, 1, wxEXPAND | wxALL, 5);

        this->SetSizer(bSizer12);
        this->Layout();

        this->Centre(wxBOTH);
        Disable();
    }
    void ManualCtrl::ConnectEvents()
    {
        m_textCtrlSpeed->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ManualCtrl::OnKillFocusSpeed), nullptr, this);
        m_textCtrlSpeed->Connect(wxEVT_COMMAND_TEXT_ENTER,
                                 wxCommandEventHandler(ManualCtrl::OnTextEnterSpeed),
                                 nullptr,
                                 this);
        m_textCtrlSpeed->Connect(wxEVT_COMMAND_TEXT_MAXLEN,
                                 wxCommandEventHandler(ManualCtrl::OnTextMaxLenSpeed),
                                 nullptr,
                                 this);
        m_sliderSpeed->Connect(wxEVT_SCROLL_THUMBRELEASE,
                               wxScrollEventHandler(ManualCtrl::OnSliderReleaseSpeed),
                               nullptr,
                               this);
        m_sliderSpeed->Connect(wxEVT_SLIDER, wxCommandEventHandler(ManualCtrl::OnSliderSpeed), nullptr, this);
        m_textCtrlAccel->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ManualCtrl::OnKillFocusAccel), nullptr, this);
        m_textCtrlAccel->Connect(wxEVT_COMMAND_TEXT_ENTER,
                                 wxCommandEventHandler(ManualCtrl::OnTextEnterAccel),
                                 nullptr,
                                 this);
        m_textCtrlAccel->Connect(wxEVT_COMMAND_TEXT_MAXLEN,
                                 wxCommandEventHandler(ManualCtrl::OnTextMaxLenAccel),
                                 nullptr,
                                 this);
        m_sliderAccel->Connect(wxEVT_SCROLL_THUMBRELEASE,
                               wxScrollEventHandler(ManualCtrl::OnSliderReleaseAccel),
                               nullptr,
                               this);
        m_sliderAccel->Connect(wxEVT_SLIDER, wxCommandEventHandler(ManualCtrl::OnSliderAccel), nullptr, this);
        m_toggleGrip01->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
                              wxCommandEventHandler(ManualCtrl::OnToggleGrip01),
                              nullptr,
                              this);
        m_toggleGrip02->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
                              wxCommandEventHandler(ManualCtrl::OnToggleGrip02),
                              nullptr,
                              this);
        m_toggleAuto->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
                              wxCommandEventHandler(ManualCtrl::OnToggleAuto),
                              nullptr,
                              this);
        m_textCtrlZ->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ManualCtrl::OnKillFocusZ), nullptr, this);
        m_textCtrlZ->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(ManualCtrl::OnTextEnterZ), nullptr, this);
        m_textCtrlZ->Connect(wxEVT_COMMAND_TEXT_MAXLEN,
                             wxCommandEventHandler(ManualCtrl::OnTextMaxLenZ),
                             nullptr,
                             this);
        m_sliderZ->Connect(wxEVT_SCROLL_THUMBRELEASE,
                           wxScrollEventHandler(ManualCtrl::OnSliderReleaseZ),
                           nullptr,
                           this);
        m_sliderZ->Connect(wxEVT_SLIDER, wxCommandEventHandler(ManualCtrl::OnSliderZ), nullptr, this);
        m_textCtrlA1->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ManualCtrl::OnKillFocusA1), nullptr, this);
        m_textCtrlA1->Connect(wxEVT_COMMAND_TEXT_ENTER,
                              wxCommandEventHandler(ManualCtrl::OnTextEnterA1),
                              nullptr,
                              this);
        m_textCtrlA1->Connect(wxEVT_COMMAND_TEXT_MAXLEN,
                              wxCommandEventHandler(ManualCtrl::OnTextMaxLenA1),
                              nullptr,
                              this);
        m_sliderA1->Connect(wxEVT_SCROLL_THUMBRELEASE,
                            wxScrollEventHandler(ManualCtrl::OnSliderReleaseA1),
                            nullptr,
                            this);
        m_sliderA1->Connect(wxEVT_SLIDER, wxCommandEventHandler(ManualCtrl::OnSliderA1), nullptr, this);
        m_textCtrlA2->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ManualCtrl::OnKillFocusA2), nullptr, this);
        m_textCtrlA2->Connect(wxEVT_COMMAND_TEXT_ENTER,
                              wxCommandEventHandler(ManualCtrl::OnTextEnterA2),
                              nullptr,
                              this);
        m_textCtrlA2->Connect(wxEVT_COMMAND_TEXT_MAXLEN,
                              wxCommandEventHandler(ManualCtrl::OnTextMaxLenA2),
                              nullptr,
                              this);
        m_sliderA2->Connect(wxEVT_SCROLL_THUMBRELEASE,
                            wxScrollEventHandler(ManualCtrl::OnSliderReleaseA2),
                            nullptr,
                            this);
        m_sliderA2->Connect(wxEVT_SLIDER, wxCommandEventHandler(ManualCtrl::OnSliderA2), nullptr, this);
        m_textCtrlA3->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ManualCtrl::OnKillFocusA3), nullptr, this);
        m_textCtrlA3->Connect(wxEVT_COMMAND_TEXT_ENTER,
                              wxCommandEventHandler(ManualCtrl::OnTextEnterA3),
                              nullptr,
                              this);
        m_textCtrlA3->Connect(wxEVT_COMMAND_TEXT_MAXLEN,
                              wxCommandEventHandler(ManualCtrl::OnTextMaxLenA3),
                              nullptr,
                              this);
        m_sliderA3->Connect(wxEVT_SCROLL_THUMBRELEASE,
                            wxScrollEventHandler(ManualCtrl::OnSliderReleaseA3),
                            nullptr,
                            this);
        m_sliderA3->Connect(wxEVT_SLIDER, wxCommandEventHandler(ManualCtrl::OnSliderA3), nullptr, this);

        this->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ManualCtrl::OnMouseRelease), nullptr, this);
        this->Connect(wxEVT_MOTION, wxMouseEventHandler(ManualCtrl::OnMouseMotion), nullptr, this);
    }
    void ManualCtrl::DisconnectEvents()
    {
        m_textCtrlSpeed->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ManualCtrl::OnKillFocusSpeed), nullptr, this);
        m_textCtrlSpeed->Disconnect(wxEVT_COMMAND_TEXT_ENTER,
                                    wxCommandEventHandler(ManualCtrl::OnTextEnterSpeed),
                                    nullptr,
                                    this);
        m_textCtrlSpeed->Disconnect(wxEVT_COMMAND_TEXT_MAXLEN,
                                    wxCommandEventHandler(ManualCtrl::OnTextMaxLenSpeed),
                                    nullptr,
                                    this);
        m_sliderSpeed->Disconnect(wxEVT_SCROLL_THUMBRELEASE,
                                  wxScrollEventHandler(ManualCtrl::OnSliderReleaseSpeed),
                                  nullptr,
                                  this);
        m_sliderSpeed->Disconnect(wxEVT_SLIDER, wxCommandEventHandler(ManualCtrl::OnSliderSpeed), nullptr, this);
        m_textCtrlAccel->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ManualCtrl::OnKillFocusAccel), nullptr, this);
        m_textCtrlAccel->Disconnect(wxEVT_COMMAND_TEXT_ENTER,
                                    wxCommandEventHandler(ManualCtrl::OnTextEnterAccel),
                                    nullptr,
                                    this);
        m_textCtrlAccel->Disconnect(wxEVT_COMMAND_TEXT_MAXLEN,
                                    wxCommandEventHandler(ManualCtrl::OnTextMaxLenAccel),
                                    nullptr,
                                    this);
        m_sliderAccel->Disconnect(wxEVT_SCROLL_THUMBRELEASE,
                                  wxScrollEventHandler(ManualCtrl::OnSliderReleaseAccel),
                                  nullptr,
                                  this);
        m_sliderAccel->Disconnect(wxEVT_SLIDER, wxCommandEventHandler(ManualCtrl::OnSliderAccel), nullptr, this);
        m_toggleGrip01->Disconnect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
                                 wxCommandEventHandler(ManualCtrl::OnToggleGrip01),
                                 nullptr,
                                 this);
        m_toggleGrip02->Disconnect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
                                 wxCommandEventHandler(ManualCtrl::OnToggleGrip02),
                                 nullptr,
                                 this);
        m_toggleAuto->Disconnect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
                                 wxCommandEventHandler(ManualCtrl::OnToggleAuto),
                                 nullptr,
                                 this);
        m_textCtrlZ->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ManualCtrl::OnKillFocusZ), nullptr, this);
        m_textCtrlZ->Disconnect(wxEVT_COMMAND_TEXT_ENTER,
                                wxCommandEventHandler(ManualCtrl::OnTextEnterZ),
                                nullptr,
                                this);
        m_textCtrlZ->Disconnect(wxEVT_COMMAND_TEXT_MAXLEN,
                                wxCommandEventHandler(ManualCtrl::OnTextMaxLenZ),
                                nullptr,
                                this);
        m_sliderZ->Disconnect(wxEVT_SCROLL_THUMBRELEASE,
                              wxScrollEventHandler(ManualCtrl::OnSliderReleaseZ),
                              nullptr,
                              this);
        m_sliderZ->Disconnect(wxEVT_SLIDER, wxCommandEventHandler(ManualCtrl::OnSliderZ), nullptr, this);
        m_textCtrlA1->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ManualCtrl::OnKillFocusA1), nullptr, this);
        m_textCtrlA1->Disconnect(wxEVT_COMMAND_TEXT_ENTER,
                                 wxCommandEventHandler(ManualCtrl::OnTextEnterA1),
                                 nullptr,
                                 this);
        m_textCtrlA1->Disconnect(wxEVT_COMMAND_TEXT_MAXLEN,
                                 wxCommandEventHandler(ManualCtrl::OnTextMaxLenA1),
                                 nullptr,
                                 this);
        m_sliderA1->Disconnect(wxEVT_SCROLL_THUMBRELEASE,
                               wxScrollEventHandler(ManualCtrl::OnSliderReleaseA1),
                               nullptr,
                               this);
        m_sliderA1->Disconnect(wxEVT_SLIDER, wxCommandEventHandler(ManualCtrl::OnSliderA1), nullptr, this);
        m_textCtrlA2->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ManualCtrl::OnKillFocusA2), nullptr, this);
        m_textCtrlA2->Disconnect(wxEVT_COMMAND_TEXT_ENTER,
                                 wxCommandEventHandler(ManualCtrl::OnTextEnterA2),
                                 nullptr,
                                 this);
        m_textCtrlA2->Disconnect(wxEVT_COMMAND_TEXT_MAXLEN,
                                 wxCommandEventHandler(ManualCtrl::OnTextMaxLenA2),
                                 nullptr,
                                 this);
        m_sliderA2->Disconnect(wxEVT_SCROLL_THUMBRELEASE,
                               wxScrollEventHandler(ManualCtrl::OnSliderReleaseA2),
                               nullptr,
                               this);
        m_sliderA2->Disconnect(wxEVT_SLIDER, wxCommandEventHandler(ManualCtrl::OnSliderA2), nullptr, this);
        m_textCtrlA3->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ManualCtrl::OnKillFocusA3), nullptr, this);
        m_textCtrlA3->Disconnect(wxEVT_COMMAND_TEXT_ENTER,
                                 wxCommandEventHandler(ManualCtrl::OnTextEnterA3),
                                 nullptr,
                                 this);
        m_textCtrlA3->Disconnect(wxEVT_COMMAND_TEXT_MAXLEN,
                                 wxCommandEventHandler(ManualCtrl::OnTextMaxLenA3),
                                 nullptr,
                                 this);
        m_sliderA3->Disconnect(wxEVT_SCROLL_THUMBRELEASE,
                               wxScrollEventHandler(ManualCtrl::OnSliderReleaseA3),
                               nullptr,
                               this);
        m_sliderA3->Disconnect(wxEVT_SLIDER, wxCommandEventHandler(ManualCtrl::OnSliderA3), nullptr, this);

        this->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ManualCtrl::OnMouseRelease), nullptr, this);
        this->Connect(wxEVT_MOTION, wxMouseEventHandler(ManualCtrl::OnMouseMotion), nullptr, this);
    }
}// namespace Frame
