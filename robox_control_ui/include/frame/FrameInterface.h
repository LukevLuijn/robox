// ======================================
// Filename:    FrameInterface.h
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        17-05-2022
// ======================================
#ifndef ROBOX_CONTROL_UI_FRAME_INTERFACE_H
#define ROBOX_CONTROL_UI_FRAME_INTERFACE_H

#include "msg_protocol.h"
#include "wx/panel.h"

namespace Frame
{
    class IFrame : public wxPanel
    {
    public:
        explicit IFrame(wxWindow* parent) : wxPanel(parent) {}
        ~IFrame() override = default;

        /**
         * @brief Get the panel of the frame.
         * 
         * @return wxPanel* 
         */
        virtual wxPanel* GetPanel() = 0;
        /**
         * @brief Update frame data.
         * 
         * @param responseType 
         */
        virtual void UpdateData(DataType_e responseType) = 0;
        /**
         * @brief Reset frame panel.
         * 
         */
        virtual void ResetPanel() = 0;
    };

    enum class FrameTypes_e : uint8_t
    {
        MANUAL_CTRL = 0,
        KEYBOARD_CTRL = 1,
        BKE_CTRL = 2,
        SETTINGS = 3,
        ABOUT = 4,
    };
    enum class Fields_e : uint8_t
    {
        FIELD_SPEED = 0,
        FIELD_ACCEL = 1,
        FIELD_Z = 2,
        FIELD_A1 = 3,
        FIELD_A2 = 4,
        FIELD_A3 = 5,
    };
}// namespace Frame

#endif//ROBOX_CONTROL_UI_FRAME_INTERFACE_H
