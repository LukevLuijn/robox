// ======================================
// Filename:    Event.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_STATE_MACHINE_TEST_STATE_MACHINE_EVENT_H
#define ROBOX_STATE_MACHINE_TEST_STATE_MACHINE_EVENT_H

#include <cstdint>

#include "config/msg_protocol.h"

namespace StateMachine
{
    class Event
    {
    public:
        Event(MessageType_e type);
        Event(const Event& other);
        virtual ~Event() = default;

        Event& operator=(const Event& other);
        bool operator==(const Event& other);
        /**
         * @brief Get the message type.
         * 
         * @see config/msg_protocols.h: MessageType_e : uint8_t
         * 
         * @return MessageType_e 
         */
        MessageType_e GetMessageType() const;

    private:
        MessageType_e m_type;
    };
}// namespace StateMachine

#endif// ROBOX_STATE_MACHINE_TEST_STATE_MACHINE_EVENT_H