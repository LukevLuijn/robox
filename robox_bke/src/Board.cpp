// ======================================
// Filename:    Board.cpp
// Project:     ROBOX BKE
// Subsystem:   BKE control
// Author:      Luke van Luijn
// Data:        09-06-2022
// ======================================

#include "Board.h"

namespace Base
{
    Board::Board()
    {
        m_squares[0] = Device::Square(HALL_SENSOR_00, HALL_SENSOR_01);
        m_squares[1] = Device::Square(HALL_SENSOR_02, HALL_SENSOR_03);
        m_squares[2] = Device::Square(HALL_SENSOR_04, HALL_SENSOR_05);
        m_squares[3] = Device::Square(HALL_SENSOR_06, HALL_SENSOR_07);
        m_squares[4] = Device::Square(HALL_SENSOR_08, HALL_SENSOR_09);
        m_squares[5] = Device::Square(HALL_SENSOR_10, HALL_SENSOR_11);
        m_squares[6] = Device::Square(HALL_SENSOR_12, HALL_SENSOR_13);
        m_squares[7] = Device::Square(HALL_SENSOR_14, HALL_SENSOR_15);
        m_squares[8] = Device::Square(HALL_SENSOR_16, HALL_SENSOR_17);
    }
    void Board::Start()
    {
        for (size_t i = 0; i < AMOUNT_OF_SQUARES; ++i)
        {
            m_squares[i].Start();
        }
    }
    void Board::GetBoard(State& rState)
    {
        for (size_t i = 0; i < AMOUNT_OF_SQUARES; ++i)
        {
            if (m_squares[i].HasPiece())
            {
                if (m_squares[i].hasPieceO())
                {
                    rState[i] = (uint8_t) Type_e::O_PIECE;
                }
                else if (m_squares[i].HasPieceX())
                {
                    rState[i] = (uint8_t) Type_e::X_PIECE;
                }
                else
                {
#ifdef DEBUG_MODE
                    Serial.println("void Board::GetBoard(State& rState)");
                    Serial.println("error, has piece but no X or O");
#endif
                    rState[i] = (uint8_t) Type_e::X_PIECE;
                }
            }
            else
            {
                rState[i] = (uint8_t) Type_e::NO_PIECE;
            }
        }
    }

}// namespace Base