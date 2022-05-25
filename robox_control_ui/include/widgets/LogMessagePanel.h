//
// Created by luke on 16-05-22.
//

#ifndef ROBOX_USER_INTERFACE_LOGMESSAGEPANEL_H
#define ROBOX_USER_INTERFACE_LOGMESSAGEPANEL_H

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

#endif//ROBOX_USER_INTERFACE_LOGMESSAGEPANEL_H
