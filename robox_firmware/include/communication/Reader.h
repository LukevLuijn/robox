// ======================================
// Filename:    Reader.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_SERIAL_TEST_READER_H
#define ROBOX_SERIAL_TEST_READER_H

#include <Arduino.h>
#include <queue>

#include "Message.h"
#include "config/config.h"

namespace Communication
{
    typedef std::queue<Communication::Message> MessageQueue;

    class Reader
    {
    public:
        Reader(const Reader& other) = delete;
        Reader& operator=(const Reader& other) = delete;
        virtual ~Reader() = default;
        /**
         * @brief Get reader instance.
         * 
         * @return Reader& The instance.
         */
        static Reader& GetInstance()
        {
            static Reader instance;
            return instance;
        }

    private:
        Reader()
        {
        }

    public:
        /**
         * @brief Continuously checks for serial messages.
         * 
         * @note This method should be called as often as possible.
         * 
         */
        void Run();
        /**
         * @brief Get the next message from the queue. 
         *        When this method is called the messages is removed from the queue.
         * 
         * @note the queue handles a FIFO system.
         * 
         * @return Communication::Message 
         */
        Communication::Message GetMessage();
        /**
         * @brief Get the type of the next message without removing it.
         * 
         * @return MessageType_e 
         */
        MessageType_e GetMessageType();
        /**
         * @brief Write messages over serial, This method should only be used to respond to requests.
         * 
         * @param string Message to write.
         */
        void WriteMessage(const String& string);
        /**
         * @brief Checks wether the message queue is empty.
         * 
         * @return true If the queue is empty.
         * @return false If the queue is NOT empty.
         */
        bool QueueEmpty();
        /**
         * @brief Clear all messages from the message queue.
         * 
         */
        void ClearMessageQueue();

    private:
        void SortMessages(const String& string);
        void ReadSerial();
        bool CheckDataMessage(const Communication::Message& message);
        void HandleDataMessage(const Communication::Message& message);

    private:
        char m_reading[MESSAGE_SIZE];
        MessageQueue m_messageQueue;
    };
}// namespace Communication


#endif// ROBOX_SERIAL_TEST_READER_H