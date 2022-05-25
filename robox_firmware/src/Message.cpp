// ======================================
// Filename:    Message.cpp
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#include "communication/Message.h"

namespace Communication
{
    // ====================================================================================
    // HEADER                                                                        HEADER
    // ====================================================================================
    Header::Header(const String& message) : m_type(static_cast<MessageType_e>(message[0])), m_index(message[1] - '0')
    {
    }
    Header::Header(const Header& other) : m_type(other.m_type), m_index(other.m_index)
    {
    }
    Header& Header::operator=(const Header& other)
    {
        if (this != &other)
        {
            m_type = other.m_type;
            m_index = other.m_index;
        }
        return *this;
    }
    bool Header::operator==(const Header& other)
    {
        return (m_type == other.m_type) && (m_index == other.m_index);
    }
    MessageType_e Header::GetType() const
    {
        return m_type;
    }
    uint16_t Header::GetIndex() const
    {
        return m_index;
    }
    // ====================================================================================
    // BODY                                                                            BODY
    // ====================================================================================
    Body::Body(const String& message)
    {
        ParseMessage(message);
    }
    Body::Body(const Body& other) : m_variables(other.m_variables), m_bufferSize(other.m_bufferSize)
    {
    }
    Body& Body::operator=(const Body& other)
    {
        if (this != &other)
        {
            for (uint8_t i = 0; i < m_bufferSize; ++i)
            {
                m_variables[i] = other.m_variables[i];
            }
            m_bufferSize = other.m_bufferSize;
        }
        return *this;
    }
    bool Body::operator==(const Body& other)
    {
        return (m_variables == other.m_variables) && (m_bufferSize == other.m_bufferSize);
    }
    String Body::GetValue(uint8_t index) const
    {
        if (index < m_bufferSize)
        {
            return m_variables[index];
        }
        else
        {
            return "";
        }
    }
    void Body::GetBuffer(Buffer& buffer, uint8_t& size) const
    {
        size = m_bufferSize;
        for (uint8_t i = 0; i < m_bufferSize; ++i)
        {
            buffer[i] = m_variables[i];
        }
        
    }
    void Body::ParseMessage(const String& message)
    {
        String content = message.substring(1, message.length());// remove brackets
        m_bufferSize = 0;

        if (content != "")
        {
            uint16_t lastIndex = 0;

            for (size_t i = 0; i < content.length(); ++i)
            {
                if (content[i] == ',')
                {
                    m_variables[m_bufferSize] = content.substring(lastIndex, i);
                    ++m_bufferSize;
                    lastIndex = i + 1;
                }
            }
            m_variables[m_bufferSize] = content.substring(lastIndex, content.length());
            ++m_bufferSize;
        }
    }
    // ====================================================================================
    // MESSAGE                                                                      MESSAGE
    // ====================================================================================
    Message::Message(const String& message)
        : m_header(message.substring(0, 2)), m_body(message.substring(2, message.length() - 1))
    {
    }
    Message::Message(const Message& other) : m_header(other.m_header), m_body(other.m_body)
    {
    }
    Message& Message::operator=(const Message& other)
    {
        if (this != &other)
        {
            m_header = other.m_header;
            m_body = other.m_body;
        }
        return *this;
    }
    bool Message::operator==(const Message& other)
    {
        return (m_header == other.m_header) && (m_body == other.m_body);
    }
    Header Message::GetHeader() const
    {
        return m_header;
    }
    Body Message::GetBody() const
    {
        return m_body;
    }
}// namespace Comm