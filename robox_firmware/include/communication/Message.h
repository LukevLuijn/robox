// ======================================
// Filename:    Message.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_CONTROLLER_COMM_MESSAGE_H
#define ROBOX_CONTROLLER_COMM_MESSAGE_H

#include <Arduino.h>
#include "config/msg_protocol.h"

namespace Communication
{
    typedef String Buffer[10];

    class Header
    {
    public:
        Header(const String& message);
        Header(const Header& other);

        virtual ~Header() = default;

        Header& operator=(const Header& other);
        bool operator==(const Header& other);

    public:
        /**
         * @brief Get the message type.
         * 
         * @return MessageType_e.
         */
        MessageType_e GetType() const;
        /**
         * @brief Get the message type index.
         * 
         * @return uint16_t message type index.
         */
        uint16_t GetIndex() const;

    public:
        MessageType_e m_type;
        uint16_t m_index;
    };

    class Body
    {
    public:
        Body(const String& message);
        Body(const Body& other);

        virtual ~Body() = default;

        Body& operator=(const Body& other);
        bool operator==(const Body& other);

    public:
        /**
         * @brief Get single value from list, based on index.
         * 
         * @param index Index of requested value.
         * @return String Actual value.
         */
        String GetValue(uint8_t index) const;
        /**
         * @brief Get the complete data buffer.
         * 
         * @param rbuffer Buffer used for the data.
         * @param rsize Size of the actually used buffer.
         */
        void GetBuffer(Buffer& rbuffer, uint8_t& rsize) const;

    private:
        void ParseMessage(const String& message);

    public:
        Buffer m_variables;
        uint8_t m_bufferSize;
    };

    class Message
    {
    public:
        Message(const String& message);
        Message(const Message& other);

        virtual ~Message() = default;

        Message& operator=(const Message& other);
        bool operator==(const Message& other);

    public:
        /**
         * @brief Get the header instance of this message.
         * 
         * @return Header The header instance.
         */
        Header GetHeader() const;
        /**
         * @brief Get the body instance of this message.
         * 
         * @return Body The body Instance.
         */
        Body GetBody() const;

    public:
        Header m_header;
        Body m_body;
    };
}// namespace Comm

#endif// ROBOX_CONTROLLER_COMM_MESSAGE_H