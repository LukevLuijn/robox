// ======================================
// Filename:    BKEDriver.h
// Project:     ROBOX Control UI
// Subsystem:   BKE control
// Author:      Luke van Luijn
// Data:        10-06-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_BKE_DRIVER_H
#define ROBOX_CONTROL_UI_BKE_DRIVER_H

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
        /**
         * @brief Start connection with BKE microcontroller.
         *
         * @param port Port name for connection.
         * @param baud Baud rate for connection.
         * @param callback Callback method for incoming responses.
         */
        void StartBKEConnection(
                const std::string& port, uint16_t baud,
                const std::function<void(const std::string&)>& callback = [](const std::string& message) {
                    INFO("Response:", message);
                });
        /**
         * @brief Stop connection with BKE microcontroller.
         */
        void StopBKEConnection();
        /**
         * @brief Callback for base SerialDriver class. Reads incoming messages.
         *
         * @param data Data read from serial.
         * @param len Length of read data.
         */
        void ReadMessage(const char* data, unsigned int len);
        /**
         * @brief Check if update contains new placed piece.
         *
         * @param piece Type of piece placed.
         * @return True if there is a new piece placed. False if not.
         */
        bool NewPiecePlaced(BKEPiece_e& piece);

    private:
        bool ParseMessage(const std::string& message);
    public:
        static Board m_board;
        static BKEResult_e m_result;

    private:
        std::function<void(const std::string&)> m_responseCallback;
        Board m_previousBoard;
    };
}// namespace Driver

#endif//ROBOX_CONTROL_UI_BKE_DRIVER_H
