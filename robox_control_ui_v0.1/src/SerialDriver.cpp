//
// Created by luke on 15-05-22.
//

#include "SerialDriver.h"

#include <utility>

#include "../include/logger/Logger.h"
#include "global_config.h"

namespace Driver
{
    /** ========================================================== **/
    /** SERIAL::WRITER------------------------------SERIAL::WRITER **/
    /** ========================================================== **/

    void SerialDriver::Writer::AddToQueue(const std::string& message)
    {
        m_messageQueue.push(message);
    }
    void SerialDriver::Writer::WriteSerial(const std::string& port, uint16_t baud,
                                           const std::function<void(const char*, size_t)>& callback)
    {
        try
        {
            CallbackAsyncSerial serial(port, baud);
            serial.setCallback(callback);

            std::vector<std::string> vars = {port, std::to_string(baud)};
            INFO("Opening serial connection", vars);
            m_isRunning = true;
            m_connectionOk = true;

            while (m_isRunning)
            {
                if (!m_messageQueue.empty())
                {
                    try
                    {
                        const std::string message = m_messageQueue.front();
                        const std::string messageString = REQUEST_START_CHAR + message+ REQUEST_STOP_CHAR;
                        serial.writeString(messageString);
                        m_messageQueue.pop();
                    }
                    catch (std::exception& e)
                    {
                        ERROR("Error during serial write", e.what());
                        m_isRunning = false;
                        m_connectionOk = false;
                    }
                    catch (...)
                    {
                        ERROR("Error during serial write", "undefined");
                        m_isRunning = false;
                        m_connectionOk = false;
                    }
                    std::this_thread::sleep_for(std::chrono::microseconds(WRITE_DELAY_US));
                }
                // prevent 100% CPU usage
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            }
            if (serial.isOpen())
            {
                serial.close();
                INFO("Serial connection closed", vars);
            }
        }
        catch( boost::wrapexcept<boost::system::system_error>& e)
        {
            std::vector<std::string> vars = {e.what(), port, std::to_string(baud)};
            ERROR("Connection refused, did you plug in the robot..?", vars);

            m_isRunning = false;
            m_connectionOk = false;
        }
    }
    bool SerialDriver::Writer::IsRunning() const
    {
        return m_isRunning;
    }
    bool SerialDriver::Writer::IsConnectionOk() const
    {
        return m_connectionOk;
    }
    void SerialDriver::Writer::StopWriter()
    {
        m_isRunning = false;
    }
    [[maybe_unused]] void SerialDriver::Writer::ReadSerial(const char* data, size_t len)
    {
        m_callback(data, len);
    }

    /** ========================================================== **/
    /** SERIAL DRIVER--------------------------------SERIAL DRIVER **/
    /** ========================================================== **/

    SerialDriver::SerialDriver() : m_port("/dev/ttyACM0"), m_baud(9600)
    {
    }
    SerialDriver::~SerialDriver()
    {
        m_writer.StopWriter();
    }
    bool SerialDriver::IsConnected() const
    {
        return m_writer.IsConnectionOk();
    }
    void SerialDriver::Start(const std::function<void(const char*, size_t)>& callback, const std::string& port,
                             uint16_t baud)
    {
        m_port = port;
        m_baud = baud;

        if (m_writer.IsRunning())
        {
            Stop();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        std::thread thread(&Writer::WriteSerial, &m_writer, m_port, m_baud, callback);
        thread.detach();
    }
    void SerialDriver::Stop()
    {
        m_writer.StopWriter();
    }
    void SerialDriver::Write(const std::string& message)
    {
        m_writer.AddToQueue(message);
    }

}// namespace Driver