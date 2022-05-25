// ======================================
// Filename:    Machine.cpp
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#include "base/Machine.h"
#include "base/Robot.h"
#include "base/States.h"
#include "communication/Reader.h"
#include "utils/Logger.h"

namespace Base
{
    void Machine::Start()
    {
        Base::Robot::GetInstance().Start();

        auto start = std::make_shared<StateStart>(*this);
        SetCurrentState(start);

        DEBUG("Setup complete", "Machine");
    }
    void Machine::Run()
    {
        HandleEventList();
        CreateEvent();

        Base::Robot::GetInstance().Run();
        Communication::Reader::GetInstance().Run();
    }
    void Machine::CreateEvent()
    {
        if (!Communication::Reader::GetInstance().QueueEmpty())
        {
            MessageType_e type = Communication::Reader::GetInstance().GetMessageType();

            // Reset message type, reset machine.
            if (type == MessageType_e::RESET)
            {
                INFO("Emergency stop detected!");

                Communication::Reader::GetInstance().ClearMessageQueue();// clear message queue
                EmptyEventQueue();                                       // clear event queue.
                ScheduleEvent(StateMachine::Event(type));                // add reset event to front of the empty queue.
            }
            else if (GetCurrentState()->ActivityComplete())
            {
                ScheduleEvent(StateMachine::Event(type));
            }
        }
    }
    void Machine::ScheduleReset()
    {
        INFO("State machine reset");

        Communication::Reader::GetInstance().ClearMessageQueue();// clear all messages from the queue.
        EmptyEventQueue();                                       // empty event queue
        ScheduleEvent(StateMachine::Event(MessageType_e::RESET));// add reset event to front of empty queue.
    }

}// namespace Base