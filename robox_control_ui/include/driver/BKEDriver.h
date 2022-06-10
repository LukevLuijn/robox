//
// Created by luke on 10-06-22.
//

#ifndef ROBOX_CONTROL_UI_BKEDRIVER_H
#define ROBOX_CONTROL_UI_BKEDRIVER_H

#include "Logger.h"
#include "SerialDriver.h"

namespace Driver
{
    typedef std::array<uint16_t , 9> Board;

    class BKEDriver : public SerialDriver
    {
    private:
        BKEDriver();
    public:
        BKEDriver(const BKEDriver& other) = delete;
        BKEDriver& operator=(const BKEDriver& other) = delete;
        ~BKEDriver() override;

        static BKEDriver& GetInstance()
        {
            static BKEDriver instance;
            return instance;
        }
    public:

        void StartBKEConnection(
                const std::string& port, uint16_t baud,
                const std::function<void(const std::string&)>& callback = [](const std::string& message) {
                    INFO("Response:", message);
                });
        void StopBKEConnection();
        void ReadMessage(const char* data, unsigned int len);

    private:
        static bool ParseMessage(const std::string& message);
    public:
        static Board m_board;
        static BKEResult_e m_result;

    private:
        std::function<void(const std::string&)> m_responseCallback;
    };
}// namespace Driver

#endif//ROBOX_CONTROL_UI_BKEDRIVER_H
