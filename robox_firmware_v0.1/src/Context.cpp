// ======================================
// Filename:    Context.cpp
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#include "state_machine/Context.h"
#include "utils/Logger.h"

namespace StateMachine
{
    void Context::ScheduleEvent(const Event& event)
    {
        m_events.push(event);
    }
    const std::shared_ptr<State>& Context::GetCurrentState() const
    {
        return m_currentState;
    }
    void Context::SetCurrentState(const std::shared_ptr<State>& state)
    {
        String previousState = "INITIAL STATE";
        if (m_currentState)
        {
            m_currentState->ExitAction();
            previousState = m_currentState->GetStateName() + " STATE";
        }

        m_currentState = state;
        m_currentState->EntryAction();

        String newState = m_currentState->GetStateName() + " STATE";
        DEBUG(previousState + " -> "+ newState);
    }
    void Context::HandleEventList()
    {
        if (!m_events.empty())
        {
            Event event = m_events.front();
            m_events.pop();
            m_currentState->HandleEvent(event, *this);
        }
        m_currentState->DoActivity();
    }
    void Context::EmptyEventQueue()
    {
        EventQueue empty;
        std::swap(m_events, empty);
        DEBUG("Event queue emptied");
    }
}// namespace StateMachine