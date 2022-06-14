// ======================================
// Filename:    FrameBuilder.h
// Project:     ROBOX Control UI
// Subsystem:   Main Application
// Author:      Luke van Luijn
// Data:        17-05-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_FRAME_BUILDER_H
#define ROBOX_CONTROL_UI_FRAME_BUILDER_H

#include "FrameInterface.h"
#include <memory>

namespace Base
{
    class FrameBuilder
    {
    public:
        FrameBuilder();
        virtual ~FrameBuilder() = default;
        /**
         * @brief Create and set current control frame.
         * 
         * @param parent Parent of the new frame.
         * @param type Type of the new frame.
         */
        void SetCurrentFrame(wxWindow* parent, Frame::FrameTypes_e type);
        /**
         * @brief Get the panel object of the current control frame.
         * 
         * @return wxPanel*
         */
        wxPanel* GetCurrentPanel();
        /**
         * @brief Trigger an update for the current control frame.
         * 
         * @param responseType Type that triggered the update.
         */
        void UpdateControlPanel(DataType_e responseType);
        /**
         * @brief Trigger an update for the current BKE-control frame.
         *
         * @param bkeType BKE type that triggered the update.
         */
        void UpdateControlPanel(BKEType_e bkeType);
        /**
         * @brief Trigger an reset for the current control frame.
         * 
         */
        void ResetControlPanel();

    private:
        Frame::IFrame* m_currentFrame{};
        Frame::FrameTypes_e m_currentType;
    };
}// namespace Base

#endif//ROBOX_CONTROL_UI_FRAME_BUILDER_H
