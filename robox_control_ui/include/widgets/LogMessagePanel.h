// ======================================
// Filename:    LogMessagePanel.h
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        16-05-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_LOG_MESSAGE_PANEL_H
#define ROBOX_CONTROL_UI_LOG_MESSAGE_PANEL_H

#include <wx/wx.h>
#include "msg_protocol.h"

namespace Widgets
{
    class LogMessagePanel : public wxPanel
    {
    public:
        LogMessagePanel(wxWindow *parent, VerbosityType_e type);
        ~LogMessagePanel() override = default;

        [[nodiscard]] VerbosityType_e GetType() const;

    private:
        void Init();

    protected:
        const wxSize IMAGE_SIZE = wxSize(32,32);

        wxStaticBitmap* m_bitmap{};
        wxStaticText* m_text{};
        VerbosityType_e m_type;
    };
}// namespace Base

#endif//ROBOX_CONTROL_UI_LOG_MESSAGE_PANEL_H
