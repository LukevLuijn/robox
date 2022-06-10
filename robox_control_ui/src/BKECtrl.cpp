//
// Created by luke on 10-06-22.
//

#include "wx/sizer.h"

#include "BKECtrl.h"
#include "Logger.h"

#include "RobotController.h"

namespace Frame
{
    BKECtrl::BKECtrl(wxWindow* parent) : IFrame(parent)
    {
        InitializeFrame();
        ConnectEvents();
    }
    BKECtrl::~BKECtrl()
    {
        DisconnectEvents();
    }
    wxPanel* BKECtrl::GetPanel()
    {
        return this;
    }
    void BKECtrl::UpdateData(DataType_e responseType)
    {
        switch (responseType)
        {
            case DataType_e::HOMING_COMPLETE:
                INFO("Homing sequence complete");
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                SyncFields();
//                Enable();
                break;
            case DataType_e::GET_STATIC:
                UpdateFields();
                break;
            case DataType_e::GET_RUNTIME:
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
    void BKECtrl::ResetPanel()
    {
        // TODO
    }
    void BKECtrl::SyncFields()
    {
        const float startSpeed = 100*100;
        const float startAccel = 100*100;

        m_fieldSpeed->SetValue(Utils::String::ToString(startSpeed / 100, 2));
        m_fieldAccel->SetValue(Utils::String::ToString(startAccel / 100, 2));

        m_sliderSpeed->SetValue(static_cast<int32_t>(startSpeed));
        m_sliderAccel->SetValue(static_cast<int32_t>(startAccel));

        Driver::RobotController::GetInstance().SetNewSpeed(startSpeed/100);
        Driver::RobotController::GetInstance().SetNewAccel(startAccel/100);

        m_sliderAccel->Refresh();
        m_sliderSpeed->Refresh();

        Refresh();
        Update();

        INFO("BKECtrl fields synchronized");
    }
    void BKECtrl::UpdateFields()
    {
        m_minMaxValues[(size_t)Fields_e::FIELD_SPEED].first = 0;
        m_minMaxValues[(size_t)Fields_e::FIELD_SPEED].second = 100;
        m_minMaxValues[(size_t)Fields_e::FIELD_ACCEL].first = 0;
        m_minMaxValues[(size_t)Fields_e::FIELD_ACCEL].second = 100;

        m_sliderSpeed->SetMin(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_SPEED].first * 100));
        m_sliderSpeed->SetMax(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_SPEED].second * 100));

        m_sliderAccel->SetMin(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_ACCEL].first * 100));
        m_sliderAccel->SetMax(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_ACCEL].second * 100));

        Refresh();
        Update();

        INFO("BKECtrl fields updated");
    }
    void BKECtrl::OnKillFocusSpeed(wxFocusEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnTextEnterSpeed(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnTextMaxLenSpeed(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnSliderReleaseSpeed(wxScrollEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnSliderSpeed(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnKillFocusAccel(wxFocusEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnTextEnterAccel(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnTextMaxLenAccel(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnSliderReleaseAccel(wxScrollEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnSliderAccel(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnClickResetBoard(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__ );
        event.Skip();
    }
    void BKECtrl::OnClickGoToHome(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnClickBoard00(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnClickBoard01(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnClickBoard02(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnClickBoard03(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnClickBoard04(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnClickBoard05(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnClickBoard06(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnClickBoard07(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::OnClickBoard08(wxCommandEvent& event)
    {
        DEBUG(__PRETTY_FUNCTION__);
        event.Skip();
    }
    void BKECtrl::InitializeFrame()
    {
        wxBoxSizer* mainSizer;
        mainSizer = new wxBoxSizer(wxVERTICAL);

        m_speedPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxFlexGridSizer* fgSizer7;
        fgSizer7 = new wxFlexGridSizer(2, 4, 0, 0);
        fgSizer7->AddGrowableCol(3);
        fgSizer7->SetFlexibleDirection(wxBOTH);
        fgSizer7->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

        m_textSpeed = new wxStaticText(m_speedPanel, wxID_ANY, wxT("Speed:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textSpeed->Wrap(-1);
        fgSizer7->Add(m_textSpeed, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

        m_fieldSpeed = new wxTextCtrl(m_speedPanel,
                                      wxID_ANY,
                                      wxEmptyString,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxTE_PROCESS_ENTER);
        fgSizer7->Add(m_fieldSpeed, 0, wxTOP | wxBOTTOM | wxLEFT, 5);

        m_iconSpeed = new wxStaticText(m_speedPanel, wxID_ANY, wxT("%"), wxDefaultPosition, wxDefaultSize, 0);
        m_iconSpeed->Wrap(-1);
        fgSizer7->Add(m_iconSpeed, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 5);

        m_sliderSpeed = new wxSlider(m_speedPanel,
                                     wxID_ANY,
                                     10000,
                                     0,
                                     10000,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxSL_HORIZONTAL);
        fgSizer7->Add(m_sliderSpeed, 0, wxEXPAND | wxTOP | wxBOTTOM | wxRIGHT, 5);

        m_textAccel = new wxStaticText(m_speedPanel, wxID_ANY, wxT("Accel:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textAccel->Wrap(-1);
        fgSizer7->Add(m_textAccel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

        m_fieldAccel = new wxTextCtrl(m_speedPanel,
                                      wxID_ANY,
                                      wxEmptyString,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxTE_PROCESS_ENTER);
        fgSizer7->Add(m_fieldAccel, 0, wxTOP | wxBOTTOM | wxLEFT, 5);

        m_iconAccel = new wxStaticText(m_speedPanel, wxID_ANY, wxT("%"), wxDefaultPosition, wxDefaultSize, 0);
        m_iconAccel->Wrap(-1);
        fgSizer7->Add(m_iconAccel, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 5);

        m_sliderAccel = new wxSlider(m_speedPanel,
                                     wxID_ANY,
                                     10000,
                                     0,
                                     10000,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxSL_HORIZONTAL);
        fgSizer7->Add(m_sliderAccel, 0, wxEXPAND | wxTOP | wxBOTTOM | wxRIGHT, 5);


        m_speedPanel->SetSizer(fgSizer7);
        m_speedPanel->Layout();
        fgSizer7->Fit(m_speedPanel);
        mainSizer->Add(m_speedPanel, 0, wxEXPAND, 5);

        m_staticLine21 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        mainSizer->Add(m_staticLine21, 0, wxEXPAND | wxALL, 5);

        m_turnPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxBoxSizer* bSizer29;
        bSizer29 = new wxBoxSizer(wxHORIZONTAL);

        m_textTurn = new wxStaticText(m_turnPanel, wxID_ANY, wxT("Turn:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textTurn->Wrap(-1);
        bSizer29->Add(m_textTurn, 0, wxALL, 5);

        m_staticLine25 = new wxStaticLine(m_turnPanel,
                                          wxID_ANY,
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxLI_HORIZONTAL | wxLI_VERTICAL);
        bSizer29->Add(m_staticLine25, 0, wxEXPAND | wxALL, 5);

        m_textTurnRobox = new wxStaticText(m_turnPanel,
                                           wxID_ANY,
                                           wxT("ROBOX"),
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxALIGN_CENTER_HORIZONTAL);
        m_textTurnRobox->Wrap(-1);
        bSizer29->Add(m_textTurnRobox, 1, wxALL, 5);

        m_staticLine24 = new wxStaticLine(m_turnPanel,
                                          wxID_ANY,
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxLI_HORIZONTAL | wxLI_VERTICAL);
        bSizer29->Add(m_staticLine24, 0, wxEXPAND | wxALL, 5);

        m_textTurnUser = new wxStaticText(m_turnPanel,
                                          wxID_ANY,
                                          wxT("User"),
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxALIGN_CENTER_HORIZONTAL);
        m_textTurnUser->Wrap(-1);
        bSizer29->Add(m_textTurnUser, 1, wxALL, 5);


        m_turnPanel->SetSizer(bSizer29);
        m_turnPanel->Layout();
        bSizer29->Fit(m_turnPanel);
        mainSizer->Add(m_turnPanel, 0, wxEXPAND, 5);

        m_staticLine22 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        mainSizer->Add(m_staticLine22, 0, wxEXPAND | wxALL, 5);

        m_boardPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxBoxSizer* bSizer30;
        bSizer30 = new wxBoxSizer(wxHORIZONTAL);

        m_panel41 = new wxPanel(m_boardPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxBoxSizer* bSizer32;
        bSizer32 = new wxBoxSizer(wxVERTICAL);

        m_buttonResetBoard = new wxButton(m_panel41, wxID_ANY, wxT("Reset board"), wxDefaultPosition, wxDefaultSize, 0);
        bSizer32->Add(m_buttonResetBoard, 1, wxALL | wxEXPAND, 5);

        m_buttonHome = new wxButton(m_panel41, wxID_ANY, wxT("Go to home"), wxDefaultPosition, wxDefaultSize, 0);
        bSizer32->Add(m_buttonHome, 1, wxALL | wxEXPAND, 5);


        m_panel41->SetSizer(bSizer32);
        m_panel41->Layout();
        bSizer32->Fit(m_panel41);
        bSizer30->Add(m_panel41, 0, wxEXPAND | wxALL, 5);

        m_panel39 = new wxPanel(m_boardPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxGridSizer* gSizer5;
        gSizer5 = new wxGridSizer(3, 3, 0, 0);

        m_buttonBoard0 = new wxBitmapButton(m_panel39,
                                            wxID_ANY,
                                            wxNullBitmap,
                                            wxDefaultPosition,
                                            wxDefaultSize,
                                            wxBU_AUTODRAW | 0);
        gSizer5->Add(m_buttonBoard0, 1, wxALL | wxEXPAND, 5);

        m_buttonBoard1 = new wxBitmapButton(m_panel39,
                                            wxID_ANY,
                                            wxNullBitmap,
                                            wxDefaultPosition,
                                            wxDefaultSize,
                                            wxBU_AUTODRAW | 0);
        gSizer5->Add(m_buttonBoard1, 1, wxALL | wxEXPAND, 5);

        m_buttonBoard2 = new wxBitmapButton(m_panel39,
                                            wxID_ANY,
                                            wxNullBitmap,
                                            wxDefaultPosition,
                                            wxDefaultSize,
                                            wxBU_AUTODRAW | 0);
        gSizer5->Add(m_buttonBoard2, 1, wxALL | wxEXPAND, 5);

        m_buttonBoard3 = new wxBitmapButton(m_panel39,
                                            wxID_ANY,
                                            wxNullBitmap,
                                            wxDefaultPosition,
                                            wxDefaultSize,
                                            wxBU_AUTODRAW | 0);
        gSizer5->Add(m_buttonBoard3, 1, wxALL | wxEXPAND, 5);

        m_buttonBoard4 = new wxBitmapButton(m_panel39,
                                            wxID_ANY,
                                            wxNullBitmap,
                                            wxDefaultPosition,
                                            wxDefaultSize,
                                            wxBU_AUTODRAW | 0);
        gSizer5->Add(m_buttonBoard4, 1, wxALL | wxEXPAND, 5);

        m_buttonBoard5 = new wxBitmapButton(m_panel39,
                                            wxID_ANY,
                                            wxNullBitmap,
                                            wxDefaultPosition,
                                            wxDefaultSize,
                                            wxBU_AUTODRAW | 0);
        gSizer5->Add(m_buttonBoard5, 1, wxALL | wxEXPAND, 5);

        m_buttonBoard6 = new wxBitmapButton(m_panel39,
                                            wxID_ANY,
                                            wxNullBitmap,
                                            wxDefaultPosition,
                                            wxDefaultSize,
                                            wxBU_AUTODRAW | 0);
        gSizer5->Add(m_buttonBoard6, 1, wxALL | wxEXPAND, 5);

        m_buttonBoard7 = new wxBitmapButton(m_panel39,
                                            wxID_ANY,
                                            wxNullBitmap,
                                            wxDefaultPosition,
                                            wxDefaultSize,
                                            wxBU_AUTODRAW | 0);
        gSizer5->Add(m_buttonBoard7, 1, wxALL | wxEXPAND, 5);

        m_buttonBoard8 = new wxBitmapButton(m_panel39,
                                            wxID_ANY,
                                            wxNullBitmap,
                                            wxDefaultPosition,
                                            wxDefaultSize,
                                            wxBU_AUTODRAW | 0);
        gSizer5->Add(m_buttonBoard8, 1, wxALL | wxEXPAND, 5);


        m_panel39->SetSizer(gSizer5);
        m_panel39->Layout();
        gSizer5->Fit(m_panel39);
        bSizer30->Add(m_panel39, 1, wxEXPAND | wxALL, 5);


        m_boardPanel->SetSizer(bSizer30);
        m_boardPanel->Layout();
        bSizer30->Fit(m_boardPanel);
        mainSizer->Add(m_boardPanel, 1, wxEXPAND, 5);

        m_staticLine23 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        mainSizer->Add(m_staticLine23, 0, wxEXPAND | wxALL, 5);

        m_statsPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxBoxSizer* bSizer33;
        bSizer33 = new wxBoxSizer(wxHORIZONTAL);

        m_panel42 = new wxPanel(m_statsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxFlexGridSizer* fgSizer8;
        fgSizer8 = new wxFlexGridSizer(2, 2, 0, 0);
        fgSizer8->AddGrowableCol(0);
        fgSizer8->AddGrowableCol(1);
        fgSizer8->SetFlexibleDirection(wxBOTH);
        fgSizer8->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

        m_textRunTime = new wxStaticText(m_panel42, wxID_ANY, wxT("Run time:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textRunTime->Wrap(-1);
        fgSizer8->Add(m_textRunTime, 1, wxALL, 5);

        m_textRunTimeValue =
                new wxStaticText(m_panel42, wxID_ANY, wxT("03:25:01"), wxDefaultPosition, wxDefaultSize, 0);
        m_textRunTimeValue->Wrap(-1);
        fgSizer8->Add(m_textRunTimeValue, 1, wxALL, 5);

        m_textGamesPlayed =
                new wxStaticText(m_panel42, wxID_ANY, wxT("Games played:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textGamesPlayed->Wrap(-1);
        fgSizer8->Add(m_textGamesPlayed, 1, wxALL, 5);

        m_textGamesPlayedValue = new wxStaticText(m_panel42, wxID_ANY, wxT("300"), wxDefaultPosition, wxDefaultSize, 0);
        m_textGamesPlayedValue->Wrap(-1);
        fgSizer8->Add(m_textGamesPlayedValue, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);


        m_panel42->SetSizer(fgSizer8);
        m_panel42->Layout();
        fgSizer8->Fit(m_panel42);
        bSizer33->Add(m_panel42, 1, wxEXPAND | wxALL, 5);

        m_staticLine28 = new wxStaticLine(m_statsPanel,
                                          wxID_ANY,
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxLI_HORIZONTAL | wxLI_VERTICAL);
        bSizer33->Add(m_staticLine28, 0, wxEXPAND | wxALL, 5);

        m_panel43 = new wxPanel(m_statsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxFlexGridSizer* fgSizer9;
        fgSizer9 = new wxFlexGridSizer(3, 5, 0, 0);
        fgSizer9->SetFlexibleDirection(wxBOTH);
        fgSizer9->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

        m_textWon = new wxStaticText(m_panel43, wxID_ANY, wxT("Won:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textWon->Wrap(-1);
        fgSizer9->Add(m_textWon, 0, wxALL, 5);

        m_textWonValue = new wxStaticText(m_panel43, wxID_ANY, wxT("100"), wxDefaultPosition, wxDefaultSize, 0);
        m_textWonValue->Wrap(-1);
        fgSizer9->Add(m_textWonValue, 0, wxALL, 5);

        m_iconWon0 = new wxStaticText(m_panel43, wxID_ANY, wxT("("), wxDefaultPosition, wxDefaultSize, 0);
        m_iconWon0->Wrap(-1);
        fgSizer9->Add(m_iconWon0, 0, wxTOP | wxBOTTOM | wxLEFT, 5);

        m_textWonPercentage = new wxStaticText(m_panel43, wxID_ANY, wxT("33.33"), wxDefaultPosition, wxDefaultSize, 0);
        m_textWonPercentage->Wrap(-1);
        fgSizer9->Add(m_textWonPercentage, 0, wxTOP | wxBOTTOM, 5);

        m_IconWon1 = new wxStaticText(m_panel43, wxID_ANY, wxT("%)"), wxDefaultPosition, wxDefaultSize, 0);
        m_IconWon1->Wrap(-1);
        fgSizer9->Add(m_IconWon1, 0, wxTOP | wxBOTTOM | wxRIGHT, 5);

        m_textLost = new wxStaticText(m_panel43, wxID_ANY, wxT("Lost:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textLost->Wrap(-1);
        fgSizer9->Add(m_textLost, 0, wxALL, 5);

        m_textLostValue = new wxStaticText(m_panel43, wxID_ANY, wxT("100"), wxDefaultPosition, wxDefaultSize, 0);
        m_textLostValue->Wrap(-1);
        fgSizer9->Add(m_textLostValue, 0, wxALL, 5);

        m_iconLost0 = new wxStaticText(m_panel43, wxID_ANY, wxT("("), wxDefaultPosition, wxDefaultSize, 0);
        m_iconLost0->Wrap(-1);
        fgSizer9->Add(m_iconLost0, 0, wxTOP | wxBOTTOM | wxLEFT, 5);

        m_textLostPercentage = new wxStaticText(m_panel43, wxID_ANY, wxT("33.33"), wxDefaultPosition, wxDefaultSize, 0);
        m_textLostPercentage->Wrap(-1);
        fgSizer9->Add(m_textLostPercentage, 0, wxTOP | wxBOTTOM, 5);

        m_iconLost1 = new wxStaticText(m_panel43, wxID_ANY, wxT("%)"), wxDefaultPosition, wxDefaultSize, 0);
        m_iconLost1->Wrap(-1);
        fgSizer9->Add(m_iconLost1, 0, wxTOP | wxBOTTOM | wxRIGHT, 5);

        m_textDraw = new wxStaticText(m_panel43, wxID_ANY, wxT("Draw:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textDraw->Wrap(-1);
        fgSizer9->Add(m_textDraw, 0, wxALL, 5);

        m_textDrawValue = new wxStaticText(m_panel43, wxID_ANY, wxT("100"), wxDefaultPosition, wxDefaultSize, 0);
        m_textDrawValue->Wrap(-1);
        fgSizer9->Add(m_textDrawValue, 0, wxALL, 5);

        m_iconDraw0 = new wxStaticText(m_panel43, wxID_ANY, wxT("("), wxDefaultPosition, wxDefaultSize, 0);
        m_iconDraw0->Wrap(-1);
        fgSizer9->Add(m_iconDraw0, 0, wxTOP | wxBOTTOM | wxLEFT, 5);

        m_textDrawPercentage = new wxStaticText(m_panel43, wxID_ANY, wxT("33.33"), wxDefaultPosition, wxDefaultSize, 0);
        m_textDrawPercentage->Wrap(-1);
        fgSizer9->Add(m_textDrawPercentage, 0, wxTOP | wxBOTTOM, 5);

        m_iconDraw1 = new wxStaticText(m_panel43, wxID_ANY, wxT("%)"), wxDefaultPosition, wxDefaultSize, 0);
        m_iconDraw1->Wrap(-1);
        fgSizer9->Add(m_iconDraw1, 0, wxTOP | wxBOTTOM | wxRIGHT, 5);


        m_panel43->SetSizer(fgSizer9);
        m_panel43->Layout();
        fgSizer9->Fit(m_panel43);
        bSizer33->Add(m_panel43, 1, wxEXPAND | wxALL, 5);


        m_statsPanel->SetSizer(bSizer33);
        m_statsPanel->Layout();
        bSizer33->Fit(m_statsPanel);
        mainSizer->Add(m_statsPanel, 0, wxEXPAND, 5);

        this->SetSizer(mainSizer);
        this->Layout();
        mainSizer->Fit(this);

//        Disable();
    }
    void BKECtrl::ConnectEvents()
    {
        m_fieldSpeed->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(BKECtrl::OnKillFocusSpeed), nullptr, this);
        m_fieldSpeed->Connect(wxEVT_COMMAND_TEXT_ENTER,
                              wxCommandEventHandler(BKECtrl::OnTextEnterSpeed),
                              nullptr,
                              this);
        m_fieldSpeed->Connect(wxEVT_COMMAND_TEXT_MAXLEN,
                              wxCommandEventHandler(BKECtrl::OnTextMaxLenSpeed),
                              nullptr,
                              this);
        m_sliderSpeed->Connect(wxEVT_SCROLL_THUMBRELEASE,
                               wxScrollEventHandler(BKECtrl::OnSliderReleaseSpeed),
                               nullptr,
                               this);
        m_sliderSpeed->Connect(wxEVT_SLIDER, wxCommandEventHandler(BKECtrl::OnSliderSpeed), nullptr, this);
        m_fieldAccel->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(BKECtrl::OnKillFocusAccel), nullptr, this);
        m_fieldAccel->Connect(wxEVT_COMMAND_TEXT_ENTER,
                              wxCommandEventHandler(BKECtrl::OnTextEnterAccel),
                              nullptr,
                              this);
        m_fieldAccel->Connect(wxEVT_COMMAND_TEXT_MAXLEN,
                              wxCommandEventHandler(BKECtrl::OnTextMaxLenAccel),
                              nullptr,
                              this);
        m_sliderAccel->Connect(wxEVT_SCROLL_THUMBRELEASE,
                               wxScrollEventHandler(BKECtrl::OnSliderReleaseAccel),
                               nullptr,
                               this);
        m_sliderAccel->Connect(wxEVT_SLIDER, wxCommandEventHandler(BKECtrl::OnSliderAccel), nullptr, this);
        m_buttonResetBoard->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                    wxCommandEventHandler(BKECtrl::OnClickResetBoard),
                                    nullptr,
                                    this);
        m_buttonHome->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                              wxCommandEventHandler(BKECtrl::OnClickGoToHome),
                              nullptr,
                              this);
        m_buttonBoard0->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                wxCommandEventHandler(BKECtrl::OnClickBoard00),
                                nullptr,
                                this);
        m_buttonBoard1->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                wxCommandEventHandler(BKECtrl::OnClickBoard01),
                                nullptr,
                                this);
        m_buttonBoard2->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                wxCommandEventHandler(BKECtrl::OnClickBoard02),
                                nullptr,
                                this);
        m_buttonBoard3->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                wxCommandEventHandler(BKECtrl::OnClickBoard03),
                                nullptr,
                                this);
        m_buttonBoard4->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                wxCommandEventHandler(BKECtrl::OnClickBoard04),
                                nullptr,
                                this);
        m_buttonBoard5->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                wxCommandEventHandler(BKECtrl::OnClickBoard05),
                                nullptr,
                                this);
        m_buttonBoard6->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                wxCommandEventHandler(BKECtrl::OnClickBoard06),
                                nullptr,
                                this);
        m_buttonBoard7->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                wxCommandEventHandler(BKECtrl::OnClickBoard07),
                                nullptr,
                                this);
        m_buttonBoard8->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                wxCommandEventHandler(BKECtrl::OnClickBoard08),
                                nullptr,
                                this);
    }
    void BKECtrl::DisconnectEvents()
    {
        m_fieldSpeed->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(BKECtrl::OnKillFocusSpeed), nullptr, this);
        m_fieldSpeed->Disconnect(wxEVT_COMMAND_TEXT_ENTER,
                                 wxCommandEventHandler(BKECtrl::OnTextEnterSpeed),
                                 nullptr,
                                 this);
        m_fieldSpeed->Disconnect(wxEVT_COMMAND_TEXT_MAXLEN,
                                 wxCommandEventHandler(BKECtrl::OnTextMaxLenSpeed),
                                 nullptr,
                                 this);
        m_sliderSpeed->Disconnect(wxEVT_SCROLL_THUMBRELEASE,
                                  wxScrollEventHandler(BKECtrl::OnSliderReleaseSpeed),
                                  nullptr,
                                  this);
        m_sliderSpeed->Disconnect(wxEVT_SLIDER, wxCommandEventHandler(BKECtrl::OnSliderSpeed), nullptr, this);
        m_fieldAccel->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(BKECtrl::OnKillFocusAccel), nullptr, this);
        m_fieldAccel->Disconnect(wxEVT_COMMAND_TEXT_ENTER,
                                 wxCommandEventHandler(BKECtrl::OnTextEnterAccel),
                                 nullptr,
                                 this);
        m_fieldAccel->Disconnect(wxEVT_COMMAND_TEXT_MAXLEN,
                                 wxCommandEventHandler(BKECtrl::OnTextMaxLenAccel),
                                 nullptr,
                                 this);
        m_sliderAccel->Disconnect(wxEVT_SCROLL_THUMBRELEASE,
                                  wxScrollEventHandler(BKECtrl::OnSliderReleaseAccel),
                                  nullptr,
                                  this);
        m_sliderAccel->Disconnect(wxEVT_SLIDER, wxCommandEventHandler(BKECtrl::OnSliderAccel), nullptr, this);
        m_buttonResetBoard->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                       wxCommandEventHandler(BKECtrl::OnClickResetBoard),
                                       nullptr,
                                       this);
        m_buttonHome->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                 wxCommandEventHandler(BKECtrl::OnClickGoToHome),
                                 nullptr,
                                 this);
        m_buttonBoard0->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard00),
                                   nullptr,
                                   this);
        m_buttonBoard1->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard01),
                                   nullptr,
                                   this);
        m_buttonBoard2->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard02),
                                   nullptr,
                                   this);
        m_buttonBoard3->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard03),
                                   nullptr,
                                   this);
        m_buttonBoard4->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard04),
                                   nullptr,
                                   this);
        m_buttonBoard5->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard05),
                                   nullptr,
                                   this);
        m_buttonBoard6->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard06),
                                   nullptr,
                                   this);
        m_buttonBoard7->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard07),
                                   nullptr,
                                   this);
        m_buttonBoard8->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard08),
                                   nullptr,
                                   this);
    }
}// namespace Frame