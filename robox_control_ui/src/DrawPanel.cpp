// ======================================
// Filename:    DrawPanel.cpp
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        20-05-2022
// ======================================

#include "DrawPanel.h"
#include "wx/wx.h"

namespace Widgets
{
    DrawPanel::DrawPanel(wxWindow* parent)
        : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
    {
        InitializeFrame();
        ConnectEvents();
    }
    DrawPanel::~DrawPanel()
    {
        DisconnectEvents();
    }
    void DrawPanel::PaintEvent(wxPaintEvent& event)
    {
        wxPaintDC dc(this);
        Render(dc);
        event.Skip();
    }
    void DrawPanel::PainNow()
    {
        wxClientDC dc (this);
        Render(dc);
    }
    void DrawPanel::OnLeaveWindow(wxMouseEvent& event)
    {
        event.Skip();
    }
    void DrawPanel::OnLeftDClick(wxMouseEvent& event)
    {
        event.Skip();
    }
    void DrawPanel::OnLeftDown(wxMouseEvent& event)
    {
        event.Skip();
    }
    void DrawPanel::OnLeftUp(wxMouseEvent& event)
    {
        event.Skip();
    }
    void DrawPanel::OnMotion(wxMouseEvent& event)
    {
        event.Skip();
    }
    void DrawPanel::OnMouseWheel(wxMouseEvent& event)
    {
        event.Skip();
    }
    void DrawPanel::InitializeFrame()
    {
    }
    void DrawPanel::ConnectEvents()
    {
        Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(DrawPanel::OnLeaveWindow), nullptr, this);
        Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(DrawPanel::OnLeftDClick), nullptr, this);
        Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(DrawPanel::OnLeftDown), nullptr, this);
        Connect(wxEVT_LEFT_UP, wxMouseEventHandler(DrawPanel::OnLeftUp), nullptr, this);
        Connect(wxEVT_MOTION, wxMouseEventHandler(DrawPanel::OnMotion), nullptr, this);
        Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(DrawPanel::OnMouseWheel), nullptr, this);
    }
    void DrawPanel::DisconnectEvents()
    {
        Disconnect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(DrawPanel::OnLeaveWindow), nullptr, this);
        Disconnect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(DrawPanel::OnLeftDClick), nullptr, this);
        Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(DrawPanel::OnLeftDown), nullptr, this);
        Disconnect(wxEVT_LEFT_UP, wxMouseEventHandler(DrawPanel::OnLeftUp), nullptr, this);
        Disconnect(wxEVT_MOTION, wxMouseEventHandler(DrawPanel::OnMotion), nullptr, this);
        Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(DrawPanel::OnMouseWheel), nullptr, this);
    }
}// namespace Frame

BEGIN_EVENT_TABLE(Widgets::DrawPanel, wxPanel)
                EVT_PAINT(DrawPanel::PaintEvent)
END_EVENT_TABLE()