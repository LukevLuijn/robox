//
// Created by luke on 10-06-22.
//

#include "BKEDriver.h"
#include "global_config.h"

namespace Driver
{
    /*static*/ Board BKEDriver::m_board;
    /*static*/ BKEResult_e BKEDriver::m_result;

    namespace
    {
        void StartSerial(BKEDriver* driver, const std::string& port, uint16_t baud)
        {
            auto callback = std::bind(&BKEDriver::ReadMessage, driver, std::placeholders::_1, std::placeholders::_2);
            driver->Start(callback, port, baud);
        }
    }// namespace

    BKEDriver::BKEDriver()
        : m_responseCallback([](const std::string& message) {
              INFO("[DEFAULT] Response:", message);
          })
    {
    }
    BKEDriver::~BKEDriver()
    {
        Stop();
    }
    void BKEDriver::StartBKEConnection(const std::string& port, uint16_t baud,
                                       const std::function<void(const std::string&)>& callback)
    {
        StartSerial(this, port, baud);
        m_responseCallback = callback;
    }
    void BKEDriver::StopBKEConnection()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        Stop();
    }
    void BKEDriver::ReadMessage(const char* data, unsigned int len)
    {
        const std::string message = std::string(data, len);

        if (message[0] == RESPONSE_START_CHAR && *(message.end() - 1) == RESPONSE_STOP_CHAR)
        {
            std::string content = message.substr(1, message.length() - 2);
            if (ParseMessage(content))
            {
                m_responseCallback(content);
            }
            else
            {
                ERROR("unable to parse", message);
            }


        }
        else
        {
            std::vector<std::string> logs = Utils::String::Divide(message, LOG_START_CHAR, LOG_STOP_CHAR);

            for (const std::string& log : logs)
            {
                Logger::Logger::GetInstance().LogExternal(log);
            }
        }
    }
    bool BKEDriver::ParseMessage(const std::string& message)
    {
        if (static_cast<MessageType_e>(message[0]) == MessageType_e::BKE)
        {
            switch (static_cast<BKEType_e>(message[1] - '0'))
            {
                case BKEType_e::BOARD_UPDATE:
                {
                    std::string params = message.substr(2);                                 // remove message header
                    Utils::String::Remove(params, std::vector<char>{'[', ']'});             // remove encasement
                    std::vector<std::string> paramList = Utils::String::Divide(params, ',');// separate values by ','

                    if (paramList.size() < BKE_UPDATE_DATA_SIZE)
                    {
                        ERROR("BKE Update message incomplete", message);
                        return false;
                    }

                    try
                    {
                        m_board[0] = static_cast<uint16_t>(std::stoi(paramList[(std::size_t) BKEUpdate_e::FIELD_0]));
                        m_board[1] = static_cast<uint16_t>(std::stoi(paramList[(std::size_t) BKEUpdate_e::FIELD_1]));
                        m_board[2] = static_cast<uint16_t>(std::stoi(paramList[(std::size_t) BKEUpdate_e::FIELD_2]));
                        m_board[3] = static_cast<uint16_t>(std::stoi(paramList[(std::size_t) BKEUpdate_e::FIELD_3]));
                        m_board[4] = static_cast<uint16_t>(std::stoi(paramList[(std::size_t) BKEUpdate_e::FIELD_4]));
                        m_board[5] = static_cast<uint16_t>(std::stoi(paramList[(std::size_t) BKEUpdate_e::FIELD_5]));
                        m_board[6] = static_cast<uint16_t>(std::stoi(paramList[(std::size_t) BKEUpdate_e::FIELD_6]));
                        m_board[7] = static_cast<uint16_t>(std::stoi(paramList[(std::size_t) BKEUpdate_e::FIELD_7]));
                        m_board[8] = static_cast<uint16_t>(std::stoi(paramList[(std::size_t) BKEUpdate_e::FIELD_8]));
                    }
                    catch (std::invalid_argument& e)
                    {
                        ERROR("exception caught", e.what());
                        return false;
                    }
                    catch (std::out_of_range& e)
                    {
                        ERROR("exception caught", e.what());
                        return false;
                    }
                    catch (...)
                    {
                        ERROR("undefined exception caught");
                        return false;
                    }
                }
                break;
                case BKEType_e::GAME_RESULT:
                {
                    std::string params = message.substr(2);                                 // remove message header
                    Utils::String::Remove(params, std::vector<char>{'[', ']'});             // remove encasement
                    std::vector<std::string> paramList = Utils::String::Divide(params, ',');// separate values by ','

                    if (paramList.size() != 1)
                    {
                        ERROR("BKE Result message incomplete", message);
                        return false;
                    }

                    try
                    {
                        m_result = static_cast<BKEResult_e>(std::stoi(paramList[0]));
                    }
                    catch (std::invalid_argument& e)
                    {
                        ERROR("exception caught", e.what());
                        return false;
                    }
                    catch (std::out_of_range& e)
                    {
                        ERROR("exception caught", e.what());
                        return false;
                    }
                    catch (...)
                    {
                        ERROR("undefined exception caught");
                        return false;
                    }
                }
                break;
                default:
                    ERROR("Undefined BKE type",message[1]);
                    return false;
            }
            return true;
        }
        ERROR("Undefined message type",message[0]);
        return false;
    }
}// namespace Driver