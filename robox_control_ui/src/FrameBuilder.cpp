// ======================================
// Filename:    FrameBuilder.cpp
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        17-05-2022
// ======================================

#include "FrameBuilder.h"
#include "ManualCtrl.h"

namespace Base
{
    FrameBuilder::FrameBuilder()
    {
    }
    void FrameBuilder::SetCurrentFrame(wxWindow* parent, Frame::FrameTypes_e type)
    {
        switch (type)
        {
            case Frame::FrameTypes_e::MANUAL_CTRL:
                m_currentFrame = new Frame::ManualCtrl(parent);
                break;
            case Frame::FrameTypes_e::KEYBOARD_CTRL:// TODO
            case Frame::FrameTypes_e::BKE_CTRL:     // TODO
            case Frame::FrameTypes_e::SETTINGS:     // TODO
            case Frame::FrameTypes_e::ABOUT:        // TODO
                break;
        }
    }
    wxPanel* FrameBuilder::GetCurrentPanel()
    {
        return m_currentFrame->GetPanel();
    }
    void FrameBuilder::UpdateControlPanel(DataType_e responseType)
    {
        m_currentFrame->UpdateData(responseType);
    }
    void FrameBuilder::ResetControlPanel()
    {
        m_currentFrame->ResetPanel();
    }

}// namespace Base