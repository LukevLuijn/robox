//
// Created by luke on 16-05-22.
//

#include "ConcreteMessagePanels.h"
#include "Bitmap.h"

namespace Widgets
{
    void PanelError::Init()
    {
        m_bitmap ->SetBitmap(Utils::Bitmap::CreateBitmap("error.png", IMAGE_SIZE));
        m_text->SetLabel(m_message.m_message);
        SetToolTip(m_message.ToString());
        SetBackgroundColour((m_message.m_sourceType == SourceType_e::INTERNAL) ? COLOR_BG_SW : COLOR_BG_FW);
        SetBackgroundColour((m_message.m_sourceType == SourceType_e::INTERNAL) ? COLOR_BG_SW : COLOR_BG_FW);
        SetForegroundColour((m_message.m_sourceType == SourceType_e::INTERNAL) ? COLOR_FG_SW : COLOR_FG_FW);
    }
    void PanelWarning::Init()
    {
        m_bitmap ->SetBitmap(Utils::Bitmap::CreateBitmap("warning.png", IMAGE_SIZE));
        m_text->SetLabel(m_message.m_message);
        SetToolTip(m_message.ToString());
        SetBackgroundColour((m_message.m_sourceType == SourceType_e::INTERNAL) ? COLOR_BG_SW : COLOR_BG_FW);
        SetBackgroundColour((m_message.m_sourceType == SourceType_e::INTERNAL) ? COLOR_BG_SW : COLOR_BG_FW);
        SetForegroundColour((m_message.m_sourceType == SourceType_e::INTERNAL) ? COLOR_FG_SW : COLOR_FG_FW);
    }
    void PanelInfo::Init()
    {
        m_bitmap ->SetBitmap(Utils::Bitmap::CreateBitmap("info.png", IMAGE_SIZE));
        m_text->SetLabel(m_message.m_message);
        SetToolTip(m_message.ToString());
        SetBackgroundColour((m_message.m_sourceType == SourceType_e::INTERNAL) ? COLOR_BG_SW : COLOR_BG_FW);
        SetBackgroundColour((m_message.m_sourceType == SourceType_e::INTERNAL) ? COLOR_BG_SW : COLOR_BG_FW);
        SetForegroundColour((m_message.m_sourceType == SourceType_e::INTERNAL) ? COLOR_FG_SW : COLOR_FG_FW);
    }
    void PanelDebug::Init()
    {
        m_bitmap ->SetBitmap(Utils::Bitmap::CreateBitmap("debug.png", IMAGE_SIZE));
        m_text->SetLabel(m_message.m_message);
        SetToolTip(m_message.ToString());
        SetBackgroundColour((m_message.m_sourceType == SourceType_e::INTERNAL) ? COLOR_BG_SW : COLOR_BG_FW);
        SetForegroundColour((m_message.m_sourceType == SourceType_e::INTERNAL) ? COLOR_FG_SW : COLOR_FG_FW);
    }
    void PanelTransition::Init()
    {
        m_bitmap ->SetBitmap(Utils::Bitmap::CreateBitmap("transition.png", IMAGE_SIZE));
        m_text->SetLabel(m_message.m_message);
        SetToolTip(m_message.ToString());
        SetBackgroundColour((m_message.m_sourceType == SourceType_e::INTERNAL) ? COLOR_BG_SW : COLOR_BG_FW);
        SetBackgroundColour((m_message.m_sourceType == SourceType_e::INTERNAL) ? COLOR_BG_SW : COLOR_BG_FW);
        SetForegroundColour((m_message.m_sourceType == SourceType_e::INTERNAL) ? COLOR_FG_SW : COLOR_FG_FW);
    }
}// namespace Base