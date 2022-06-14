// ======================================
// Filename:    FrameBuilder.cpp
// Project:     ROBOX Control UI
// Subsystem:   Main Application
// Author:      Luke van Luijn
// Data:        17-05-2022
// ======================================

#include "FrameBuilder.h"
#include "BKECtrl.h"
#include "ManualCtrl.h"
#include "Logger.h"

namespace Base
{
    FrameBuilder::FrameBuilder() : m_currentType(Frame::FrameTypes_e::MANUAL_CTRL)
    {
    }
    void FrameBuilder::SetCurrentFrame(wxWindow* parent, Frame::FrameTypes_e type)
    {
        switch (type)
        {
            case Frame::FrameTypes_e::MANUAL_CTRL:
                m_currentFrame = new Frame::ManualCtrl(parent);
                break;
            case Frame::FrameTypes_e::BKE_CTRL:
                m_currentFrame = new Frame::BKECtrl(parent);
                break;
            case Frame::FrameTypes_e::KEYBOARD_CTRL:// TODO
            case Frame::FrameTypes_e::SETTINGS:     // TODO
            case Frame::FrameTypes_e::ABOUT:        // TODO
                break;
        }
        m_currentType = type;
    }
    wxPanel* FrameBuilder::GetCurrentPanel()
    {
        return m_currentFrame->GetPanel();
    }
    void FrameBuilder::UpdateControlPanel(DataType_e responseType)
    {
        m_currentFrame->UpdateData(responseType);
    }
    void FrameBuilder::UpdateControlPanel(BKEType_e bkeType)
    {
        if (m_currentType == Frame::FrameTypes_e::BKE_CTRL)
        {
            auto pCurrentFrame = dynamic_cast<Frame::BKECtrl*>(m_currentFrame);
            if (pCurrentFrame != nullptr)
            {
                pCurrentFrame->UpdateBKE(bkeType);
            }
            else
            {
                WARNING("could not perform dynamic cast");
            }
        }
    }
    void FrameBuilder::ResetControlPanel()
    {
        m_currentFrame->ResetPanel();
    }

}// namespace Base