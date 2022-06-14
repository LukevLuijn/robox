// ======================================
// Filename:    LogMessagePanel.cpp
// Project:     ROBOX Control UI
// Subsystem:   Main Application
// Author:      Luke van Luijn
// Data:        16-05-2022
// ======================================

#include "LogMessagePanel.h"

namespace Widgets
{
    LogMessagePanel::LogMessagePanel(wxWindow* parent, VerbosityType_e type)
        : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL), m_type(type)
    {
        Init();
    }
    void LogMessagePanel::Init()
    {
        wxBoxSizer* sizer;
        sizer = new wxBoxSizer(wxHORIZONTAL);
        m_text = new wxStaticText(this, wxID_ANY, wxT("Default message"), wxDefaultPosition, wxDefaultSize, 0);
        m_bitmap = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize);
        sizer->Add(m_bitmap, 0, wxALL, 5);
        sizer->Add(m_text, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
        m_text->Wrap(-1);

        SetSizer(sizer);
        Layout();
        sizer->Fit(this);
    }
    VerbosityType_e LogMessagePanel::GetType() const
    {
        return m_type;
    }


}// namespace Widgets