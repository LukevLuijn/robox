// ======================================
// Filename:    DrawPanel.h
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        20-05-2022
// ======================================

#ifndef ROBOT_VISUAL_DRAWPANEL_H
#define ROBOT_VISUAL_DRAWPANEL_H

#include "wx/panel.h"

namespace Widgets
{
    class DrawPanel : public wxPanel
    {
    public:
        explicit DrawPanel(wxWindow* parent);
        ~DrawPanel() override;

        /**
         * @brief Handle paint event.
         * 
         * @note this method is called by main thread.
         * 
         * @param event 
         */
        void PaintEvent(wxPaintEvent& event);
        /**
         * @brief Trigger a paint event.
         * 
         */
        void PainNow();
        /**
         * @brief Render a panel using wxDC.
         * 
         * @param dc 
         */
        virtual void Render(wxDC& dc) = 0;

    protected:
        virtual void OnLeaveWindow(wxMouseEvent& event);
        virtual void OnLeftDClick(wxMouseEvent& event);
        virtual void OnLeftDown(wxMouseEvent& event);
        virtual void OnLeftUp(wxMouseEvent& event);
        virtual void OnMotion(wxMouseEvent& event);
        virtual void OnMouseWheel(wxMouseEvent& event);
        void EraseBackground(wxEraseEvent& WXUNUSED(event)) {}

    private:
        void InitializeFrame();
        void ConnectEvents();
        void DisconnectEvents();

        DECLARE_EVENT_TABLE()
    };
}// namespace Frame



#endif//ROBOT_VISUAL_DRAWPANEL_H
