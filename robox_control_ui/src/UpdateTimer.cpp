//
// Created by luke on 12-06-22.
//

#include "UpdateTimer.h"
#include "Logger.h"


namespace Utils
{
    wxDEFINE_EVENT(UPDATE_EVENT, wxCommandEvent);

    UpdateTimer::UpdateTimer(wxPanel* parent)
    : wxTimer(parent)
    {
    }
    void UpdateTimer::Notify()
    {
        wxCommandEvent event(UPDATE_EVENT);
        wxPostEvent(GetOwner(), event);
    }
    bool UpdateTimer::StartTimer(uint32_t time)
    {
        return wxTimer::Start(static_cast<int>(time));
    }
}// namespace Utils