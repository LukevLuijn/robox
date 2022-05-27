// ======================================
// Filename:    MainFrame.cpp
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        17-05-2022
// ======================================

#include "MainFrame.h"
#include "Bitmap.h"
#include "global_config.h"

namespace Frame
{
    MainFrame::MainFrame(wxWindow* parent) : wxFrame(parent, wxID_ANY, WINDOW_TITLE, wxDefaultPosition, wxSize(WINDOW_WIDTH,WINDOW_HEIGHT), wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL)
    {
        InitializeFrame();
        ConnectEvents();
    }

    MainFrame::~MainFrame()
    {
        DisconnectEvents();
    }
    void MainFrame::OnCloseWindow( wxCloseEvent& event )
    {
        event.Skip();
    }
    void MainFrame::OnClickManual(wxCommandEvent& event)
    {
        event.Skip();
    }
    void MainFrame::OnClickKeyboard(wxCommandEvent& event)
    {
        event.Skip();
    }
    void MainFrame::OnClickBKE(wxCommandEvent& event)
    {
        event.Skip();
    }
    void MainFrame::OnClickSettings(wxCommandEvent& event)
    {
        event.Skip();
    }
    void MainFrame::OnClickAbout(wxCommandEvent& event)
    {
        event.Skip();
    }
    void MainFrame::OnClickMenuConnect(wxCommandEvent& event)
    {
        event.Skip();
    }
    void MainFrame::OnClickMenuActivate(wxCommandEvent& event)
    {
        event.Skip();
    }
    void MainFrame::OnClickMenuRun(wxCommandEvent& event)
    {
        event.Skip();
    }
    void MainFrame::OnClickMenuStop(wxCommandEvent& event)
    {
        event.Skip();
    }
    void MainFrame::OnClickMenuEStop(wxCommandEvent& event)
    {
        event.Skip();
    }
    void MainFrame::InitializeFrame()
    {
        this->SetSizeHints( wxSize(550,500), wxDefaultSize );

        wxBoxSizer* m_mainSizer = new wxBoxSizer( wxHORIZONTAL );

        m_menuPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        wxBoxSizer* menuSizer;
        menuSizer = new wxBoxSizer( wxVERTICAL );

        m_menuButtons[(uint8_t)FrameTypes_e::MANUAL_CTRL] = new wxButton( m_menuPanel, wxID_ANY, wxT("Manual"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE );
        menuSizer->Add( m_menuButtons[(uint8_t)FrameTypes_e::MANUAL_CTRL], 0, wxEXPAND|wxBOTTOM, 5 );

        m_menuButtons[(uint8_t)FrameTypes_e::KEYBOARD_CTRL] = new wxButton( m_menuPanel, wxID_ANY, wxT("Keyboard"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE );
        menuSizer->Add( m_menuButtons[(uint8_t)FrameTypes_e::KEYBOARD_CTRL], 0, wxEXPAND|wxBOTTOM, 5 );

        m_menuButtons[(uint8_t)FrameTypes_e::BKE_CTRL] = new wxButton( m_menuPanel, wxID_ANY, wxT("BKE-ctrl"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE );
        menuSizer->Add( m_menuButtons[(uint8_t)FrameTypes_e::BKE_CTRL], 0, wxEXPAND|wxBOTTOM, 5 );

        m_menuLine0 = new wxStaticLine( m_menuPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
        menuSizer->Add( m_menuLine0, 0, wxEXPAND | wxALL, 5 );

        m_menuButtons[(uint8_t)FrameTypes_e::SETTINGS] = new wxButton( m_menuPanel, wxID_ANY, wxT("Settings"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE );
        menuSizer->Add( m_menuButtons[(uint8_t)FrameTypes_e::SETTINGS], 0, wxEXPAND|wxBOTTOM, 5 );

        m_menuButtons[(uint8_t)FrameTypes_e::ABOUT] = new wxButton( m_menuPanel, wxID_ANY, wxT("About"), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE );
        menuSizer->Add( m_menuButtons[(uint8_t)FrameTypes_e::ABOUT] , 0, wxEXPAND|wxBOTTOM, 5 );

        m_menuLine1 = new wxStaticLine( m_menuPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
        menuSizer->Add( m_menuLine1, 0, wxEXPAND | wxALL, 5 );

        m_buttonMenuConnect = new wxToggleButton( m_menuPanel, wxID_ANY, wxT("Connect"), wxDefaultPosition, wxDefaultSize, 0 );
        menuSizer->Add( m_buttonMenuConnect, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

        m_buttonMenuActivate = new wxToggleButton( m_menuPanel, wxID_ANY, wxT("Activate"), wxDefaultPosition, wxDefaultSize, 0 );
        menuSizer->Add( m_buttonMenuActivate, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );

        m_buttonMenuRun = new wxButton( m_menuPanel, wxID_ANY, wxT("Run"), wxDefaultPosition, wxDefaultSize, 0 );
        menuSizer->Add( m_buttonMenuRun, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );


        wxBitmap bitmapStop = Utils::Bitmap::CreateBitmap("stop.png", 64, 64, true);
        m_buttonMenuStop = new wxBitmapButton( m_menuPanel, wxID_ANY, bitmapStop, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
        menuSizer->Add( m_buttonMenuStop, 1, wxEXPAND|wxTOP|wxBOTTOM, 5 );

        wxBitmap bitmapEStop = Utils::Bitmap::CreateBitmap("estop.png", 64, 64, true);
        m_buttonMenuEStop = new wxBitmapButton( m_menuPanel, wxID_ANY, bitmapEStop, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );
        menuSizer->Add( m_buttonMenuEStop, 1, wxEXPAND|wxTOP|wxBOTTOM, 5 );


        m_menuPanel->SetSizer( menuSizer );
        m_menuPanel->Layout();
        menuSizer->Fit( m_menuPanel );
        m_mainSizer->Add( m_menuPanel, 0, wxALL|wxEXPAND, 5 );

        m_menuLine2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL|wxLI_VERTICAL );
        m_mainSizer->Add( m_menuLine2, 0, wxEXPAND | wxALL, 5 );

        m_placeholder = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        m_placeholderSizer = new wxBoxSizer( wxVERTICAL );

        m_placeholder->SetSizer( m_placeholderSizer );
        m_placeholder->Layout();
        m_placeholderSizer->Fit( m_placeholder );

        m_mainSizer->Add( m_placeholder, 1, wxEXPAND, 5 );

        m_menuLine1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
        m_mainSizer->Add( m_menuLine1, 0, wxEXPAND | wxALL, 5 );

        m_logPanel = new Widgets::LogPanel(this);
        m_mainSizer->Add(m_logPanel, 0, wxEXPAND | wxALL, 5);

        this->SetSizer( m_mainSizer );
        this->Layout();
        this->Centre( wxBOTH );

        m_buttonMenuActivate->Disable();
        m_buttonMenuRun->Disable();
        m_buttonMenuStop->Disable();
        m_buttonMenuEStop->Disable();
    }
    void MainFrame::ConnectEvents()
    {
        this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnCloseWindow ) );

        m_menuButtons[(size_t)FrameTypes_e::MANUAL_CTRL]->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickManual ), nullptr, this );
        m_menuButtons[(size_t)FrameTypes_e::KEYBOARD_CTRL]->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickKeyboard ), nullptr, this );
        m_menuButtons[(size_t)FrameTypes_e::BKE_CTRL]->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickBKE ), nullptr, this );
        m_menuButtons[(size_t)FrameTypes_e::SETTINGS]->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickSettings ), nullptr, this );
        m_menuButtons[(size_t)FrameTypes_e::ABOUT]->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickAbout ), nullptr, this );

        m_buttonMenuConnect->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickMenuConnect ), nullptr, this );
        m_buttonMenuActivate->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickMenuActivate ), nullptr, this );
        m_buttonMenuRun->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickMenuRun ), nullptr, this );
        m_buttonMenuStop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickMenuStop ), nullptr, this );
        m_buttonMenuEStop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickMenuEStop ), nullptr, this );
    }
    void MainFrame::DisconnectEvents()
    {
        this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainFrame::OnCloseWindow ) );

        m_menuButtons[(size_t)FrameTypes_e::MANUAL_CTRL]->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickManual ), nullptr, this );
        m_menuButtons[(size_t)FrameTypes_e::KEYBOARD_CTRL]->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickKeyboard ), nullptr, this );
        m_menuButtons[(size_t)FrameTypes_e::BKE_CTRL]->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickBKE ), nullptr, this );
        m_menuButtons[(size_t)FrameTypes_e::SETTINGS]->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickSettings ), nullptr, this );
        m_menuButtons[(size_t)FrameTypes_e::ABOUT]->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickAbout ), nullptr, this );

        m_buttonMenuConnect->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickMenuConnect ), nullptr, this );
        m_buttonMenuActivate->Disconnect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickMenuActivate ), nullptr, this );
        m_buttonMenuRun->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickMenuRun ), nullptr, this );
        m_buttonMenuStop->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickMenuStop ), nullptr, this );
        m_buttonMenuEStop->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrame::OnClickMenuEStop ), nullptr, this );
    }

}// namespace Frame