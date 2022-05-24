// ======================================
// Filename:    State.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_STATE_MACHINE_TEST_STATE_MACHINE_STATE_H
#define ROBOX_STATE_MACHINE_TEST_STATE_MACHINE_STATE_H

#include <Arduino.h>

#include "Event.h"

namespace StateMachine
{
    class Context;

    class State
    {
    public:
        State(String name) : m_activityComplete(false), m_stateName(name)
        {
        }
        State(const State& state) = delete;
        virtual ~State() = default;

        /**
         * @brief Pure virtual, handles given event based on context.
         * 
         * @param event     Event to handle.
         * @param context   Context to base event on.
         * @return true     If event was handled.
         * @return false    If event was not handled.
         */
        virtual bool HandleEvent(const Event& event, Context& context) = 0;
        /**
         * @brief Pure virtual, Action to perform after state transition.
         * 
         */
        virtual void EntryAction() = 0;
        /**
         * @brief Pure virtual, Activity to perform while state is active.
         * 
         */
        virtual void DoActivity() = 0;
        /**
         * @brief Pure virtual, Action to perform before state transition.
         * 
         */
        virtual void ExitAction() = 0;
        /**
         * @brief Check if the activity is complete.
         * 
         * @return true If the activity is complete.
         * @return false If the activity is NOT complete.
         */
        bool ActivityComplete()
        {
            return m_activityComplete;
        }
        /**
         * @brief Get the name of the state.
         * 
         * @return String Name of the state.
         */
        String GetStateName()
        {
            return m_stateName;
        }

    protected:
        bool m_activityComplete;
        String m_stateName;
    };
}// namespace StateMachine

#endif// ROBOX_STATE_MACHINE_TEST_STATE_MACHINE_STATE_H