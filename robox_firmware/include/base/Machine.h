// ======================================
// Filename:    Machine.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_CONTROLLER_BASE_MACHINE_H
#define ROBOX_CONTROLLER_BASE_MACHINE_H

#include "state_machine/Context.h"

namespace Base
{
    class Machine : public StateMachine::Context
    {
    public:
        Machine() = default;
        ~Machine() override = default;

        /**
         * @brief Start machine instance, creates initial state (start state).
         * 
         */
        void Start();
        /**
         * @brief Continuously run machine.
         * 
         * @note Call as often as posible.
         * 
         */
        void Run();
        /**
         * @brief Creates a event from incoming messages, if available.
         * 
         */
        void CreateEvent();
        /**
         * @brief Schedule a reset of the state machine.
         * 
         */
        void ScheduleReset();
    };
}// namespace Base

#endif// ROBOX_CONTROLLER_BASE_MACHINE_H