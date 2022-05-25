// ======================================
// Filename:    CircularBuffer.cpp
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#include "utils/CircularBuffer.h"

namespace Utils
{
    CBuffer::CBuffer() : m_bufferIndex(0), m_bufferFilledOnce(false)
    {
        FillBufferWith(-1);
    }
    CBuffer::CBuffer(const CBuffer& other)
        : m_bufferIndex(other.m_bufferIndex), m_bufferFilledOnce(other.m_bufferFilledOnce)
    {
        CopyBuffer(other.m_buffer);
    }
    CBuffer& CBuffer::operator=(const CBuffer& other)
    {
        if (this != &other)
        {
            m_bufferIndex = other.m_bufferIndex;
            m_bufferFilledOnce = other.m_bufferFilledOnce;
            CopyBuffer(other.m_buffer);
        }
        return *this;
    }
    bool CBuffer::operator==(const CBuffer& other) const
    {
        return (m_bufferIndex == other.m_bufferIndex) && (m_bufferFilledOnce == other.m_bufferFilledOnce) &&
               CompareBuffer(other.m_buffer);
    }
    bool CBuffer::IsEmpty()
    {
        return (!m_bufferFilledOnce && m_bufferIndex == 0);
    }
    void CBuffer::Clear()
    {
        FillBufferWith(-1);
        m_bufferIndex = 0;
        m_bufferFilledOnce = false;
    }
    void CBuffer::Push(uint8_t value)
    {
        m_buffer[m_bufferIndex] = value;

        ++m_bufferIndex;
        if (m_bufferIndex >= MAX_BUFFER_SIZE)
        {
            m_bufferIndex = 0;
            m_bufferFilledOnce = true;
        }
    }
    bool CBuffer::GetValue(size_t index, uint8_t& value)
    {
        size_t actualBufferSize = (m_bufferFilledOnce) ? MAX_BUFFER_SIZE : m_bufferIndex;

        if (index >= actualBufferSize)
        {
            value = 0;
            return false;
        }
        else
        {
            value = m_buffer[index];
            return true;
        }
    }
    float CBuffer::GetAverage()
    {
        size_t actualBufferSize = (m_bufferFilledOnce) ? MAX_BUFFER_SIZE : m_bufferIndex;
        float average = -1;
        if (actualBufferSize > 0)
        {
            float sum = 0.0f;
            for (size_t i = 0; i < actualBufferSize; ++i)
            {
                sum += m_buffer[i];
            }
            average = round(sum / (float)actualBufferSize);
        }
        return average;
    }
    void CBuffer::FillBufferWith(int16_t value)
    {
        for (size_t i = 0; i < MAX_BUFFER_SIZE; ++i)
        {
            m_buffer[i] = value;
        }
    }
    void CBuffer::CopyBuffer(const CircularBuffer& other)
    {
        for (size_t i = 0; i < MAX_BUFFER_SIZE; ++i)
        {
            m_buffer[i] = other[i];
        }
    }
    bool CBuffer::CompareBuffer(const CircularBuffer& other) const
    {
        bool equals = true;
        for (size_t i = 0; i < MAX_BUFFER_SIZE; ++i)
        {
            equals = (m_buffer[i] == other[i]) ? equals : false;
        }
        return equals;
    }

}// namespace Utils