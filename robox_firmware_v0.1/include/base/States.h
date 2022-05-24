// ======================================
// Filename:    States.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_CONTROLLER_BASE_STATES_H
#define ROBOX_CONTROLLER_BASE_STATES_H

#include "Machine.h"
#include "state_machine/State.h"

namespace Base
{
    class MachineState : public StateMachine::State
    {
    public:
        ~MachineState() override = default;

    protected:
        MachineState(Machine& machine, String name) : State(name), m_machine(machine)
        {
        }

    protected:
        Machine& m_machine;
    };
    class StateStart : public MachineState
    {
    public:
        explicit StateStart(Machine& machine) : MachineState(machine, "START")
        {
        }
        ~StateStart() override = default;

        /**
         * @brief Event handler, executes continuously.
         * 
         * @param event Current event.
         * @param context Reference to machine context.
         * @return true If event was handled.
         * @return false If event was skipped.
         */
        bool HandleEvent(const StateMachine::Event& event, StateMachine::Context& context) override;
        /**
         * @brief Entry action, executes once.
         * 
         */
        virtual void EntryAction() override;
        /**
         * @brief Do activity, executes continuously.
         * 
         */
        virtual void DoActivity() override;
        /**
         * @brief Exit action, exectues once.
         * 
         */
        virtual void ExitAction() override;
    };
    class StateHome : public MachineState
    {
    public:
        explicit StateHome(Machine& machine) : MachineState(machine, "HOME")
        {
        }
        ~StateHome() override = default;

        /**
         * @brief Event handler, executes continuously.
         * 
         * @param event Current event.
         * @param context Reference to machine context.
         * @return true If event was handled.
         * @return false If event was skipped.
         */
        bool HandleEvent(const StateMachine::Event& event, StateMachine::Context& context) override;
        /**
         * @brief Entry action, executes once.
         * 
         */
        virtual void EntryAction() override;
        /**
         * @brief Do activity, executes continuously.
         * 
         */
        virtual void DoActivity() override;
        /**
         * @brief Exit action, exectues once.
         * 
         */
        virtual void ExitAction() override;
    };
    class StateMove : public MachineState
    {
    public:
        explicit StateMove(Machine& machine) : MachineState(machine, "MOVE")
        {
        }
        ~StateMove() override = default;

        /**
         * @brief Event handler, executes continuously.
         * 
         * @param event Current event.
         * @param context Reference to machine context.
         * @return true If event was handled.
         * @return false If event was skipped.
         */
        bool HandleEvent(const StateMachine::Event& event, StateMachine::Context& context) override;
        /**
         * @brief Entry action, executes once.
         * 
         */
        virtual void EntryAction() override;
        /**
         * @brief Do activity, executes continuously.
         * 
         */
        virtual void DoActivity() override;
        /**
         * @brief Exit action, exectues once.
         * 
         */
        virtual void ExitAction() override;
    };
    class StateStop : public MachineState
    {
    public:
        explicit StateStop(Machine& machine) : MachineState(machine, "STOP")
        {
        }
        ~StateStop() override = default;

        /**
         * @brief Event handler, executes continuously.
         * 
         * @param event Current event.
         * @param context Reference to machine context.
         * @return true If event was handled.
         * @return false If event was skipped.
         */
        bool HandleEvent(const StateMachine::Event& event, StateMachine::Context& context) override;
        /**
         * @brief Entry action, executes once.
         * 
         */
        virtual void EntryAction() override;
        /**
         * @brief Do activity, executes continuously.
         * 
         */
        virtual void DoActivity() override;
        /**
         * @brief Exit action, exectues once.
         * 
         */
        virtual void ExitAction() override;

    private:
        StopType_e m_index;
        uint32_t m_pauseTimer, m_pauseInterval;
    };
}// namespace Base

#endif// ROBOX_CONTROLLER_BASE_STATES_H
