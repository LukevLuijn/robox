// ======================================
// Filename:    Event.cpp
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#include "state_machine/Event.h"

namespace StateMachine
{

    Event::Event(MessageType_e type) : m_type(type)
    {
    }
    Event::Event(const Event& other) : m_type(other.m_type)
    {
    }
    Event& Event::operator=(const Event& other)
    {
        if (this != &other)
        {
            m_type = other.m_type;
        }
        return *this;
    }
    bool Event::operator==(const Event& other)
    {
        return m_type == other.m_type;
    }
    MessageType_e Event::GetMessageType() const
    {
        return m_type;
    }

}// namespace StateMachine