//
// Created by luke on 12-06-22.
//

#ifndef ROBOX_CONTROL_UI_UPDATETIMER_H
#define ROBOX_CONTROL_UI_UPDATETIMER_H

#include "wx/timer.h"
#include "wx/panel.h"

namespace Utils
{
    class UpdateTimer : public wxTimer
    {
    public:
        explicit UpdateTimer(wxPanel* parent);
        void Notify() override;
        bool StartTimer(uint32_t time);
    };
}

#endif//ROBOX_CONTROL_UI_UPDATETIMER_H
