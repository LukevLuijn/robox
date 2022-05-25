// ======================================
// Filename:    Context.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_STATE_MACHINE_TEST_STATE_MACHINE_CONTEXT_H
#define ROBOX_STATE_MACHINE_TEST_STATE_MACHINE_CONTEXT_H

#include <memory>
#include <queue>

#include "Event.h"
#include "State.h"

namespace StateMachine
{
    typedef std::queue<Event> EventQueue;

    class Context
    {
    public:
        Context() = default;
        Context(const Context& other) = delete;
        virtual ~Context() = default;

        /**
         * @brief Schedule new event for the state machine.
         * 
         * @param event Newly scheduled event.
         */
        void ScheduleEvent(const Event& event);
        /**
         * @brief Get the current state.
         * 
         * @return const std::shared_ptr<State>& pointer to the current state.
         */
        const std::shared_ptr<State>& GetCurrentState() const;
        /**
         * @brief Set the current state.
         * 
         * @param state New state to set as current.
         */
        void SetCurrentState(const std::shared_ptr<State>& state);
        /**
         * @brief Work off all queue'ed events.
         * 
         */
        void HandleEventList();
        /**
         * @brief Clear all events from the event queue.
         * 
         */
        void EmptyEventQueue();

    private:
        std::shared_ptr<State> m_currentState;
        EventQueue m_events;
    };
}// namespace StateMachine

#endif// ROBOX_STATE_MACHINE_TEST_STATE_MACHINE_CONTEXT_H
