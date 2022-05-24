// ======================================
// Filename:    CircularBuffer.h
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#ifndef ROBOX_CONTROLLER_UTILS_CIRC_BUFFER_H
#define ROBOX_CONTROLLER_UTILS_CIRC_BUFFER_H

#include <Arduino.h>

namespace
{
#define MAX_BUFFER_SIZE 100
}// namespace

namespace Utils
{
    class CBuffer
    {
    private:
        typedef int16_t CircularBuffer[MAX_BUFFER_SIZE];

    public:
        CBuffer();
        CBuffer(const CBuffer& other);
        virtual ~CBuffer() = default;

        CBuffer& operator=(const CBuffer& other);
        bool operator==(const CBuffer& other) const;

        /**
         * @brief Check wether the buffer is empty.
         * 
         * @return true If the buffer is empty.
         * @return false If the buffer is NOT empty.
         */
        bool IsEmpty();
        /**
         * @brief Remove all entries from the buffer.
         * 
         */
        void Clear();
        /**
         * @brief Add a value to the buffer.
         * 
         * @param value 
         */
        void Push(uint8_t value);
        /**
         * @brief Get a single value from the buffer.
         * 
         * @param index Index of the requested value.
         * @param value Reference to the value.
         * @return true If the requested index contains a value.
         * @return false If the requested index did not catain a value.
         */
        bool GetValue(size_t index, uint8_t& value);
        /**
         * @brief Get the average value of the buffer.
         * 
         * @return float The average value.
         */
        float GetAverage();
    private:
        void FillBufferWith(int16_t value);
        void CopyBuffer(const CircularBuffer& other);
        bool CompareBuffer(const CircularBuffer& other) const;

    private:
        CircularBuffer m_buffer;
        uint8_t m_bufferIndex;
        bool m_bufferFilledOnce;
    };
}// namespace Utils

#endif//ROBOX_CONTROLLER_UTILS_CIRC_BUFFER_H
