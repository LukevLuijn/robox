// ======================================
// Filename:    SerialDriver.h
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        15-05-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_SERIAL_DRIVER_H
#define ROBOX_CONTROL_UI_SERIAL_DRIVER_H

#include <queue>

#include "AsyncSerial.h"

namespace Driver
{
    class SerialDriver : public boost::noncopyable
    {
    private:
        struct Writer {
            void AddToQueue(const std::string& message);
            void WriteSerial(const std::string& port, uint16_t baud,
                             const std::function<void(const char*, size_t)>& callback);

            [[nodiscard]] bool IsRunning() const;
            [[nodiscard]] bool IsConnectionOk() const;

            void StopWriter();

        private:
            [[maybe_unused]] void ReadSerial(const char* data, size_t len);

        private:
            std::queue<std::string> m_messageQueue;
            std::function<void(const char*, size_t)> m_callback;
            bool m_isRunning = true;
            bool m_connectionOk = false;
        };

    public:
        SerialDriver();
        virtual ~SerialDriver();
        /**
         * @brief Check if the serial driver is currently connected to the device.
         * 
         * @return true If the driver is currently connected.
         * @return false If the driver is currently NOT connected.
         */
        [[nodiscard]] bool IsConnected() const;
        /**
         * @brief Start the connection over serial.
         * 
         * @param callback Callback for the incoming messages.
         * @param port Serial port.
         * @param baud BAUD rate.
         */
        void Start(const std::function<void(const char*, size_t)>& callback, const std::string& port = "/dev/ttyACM0",
                   uint16_t baud = 9600);
        /**
         * @brief Stop the connection over serial.
         * 
         */
        void Stop();

    protected:
        void Write(const std::string& message);


    private:
        Writer m_writer;
        std::string m_port;
        uint16_t m_baud;
    };
}// namespace Driver

#endif//ROBOX_CONTROL_UI_SERIAL_DRIVER_H
