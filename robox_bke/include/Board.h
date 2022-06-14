// ======================================
// Filename:    Board.h
// Project:     ROBOX BKE
// Subsystem:   BKE control
// Author:      Luke van Luijn
// Data:        09-06-2022
// ======================================

#ifndef ROBOX_BKE_BOARD_H
#define ROBOX_BKE_BOARD_H

#include "Square.h"
#include "config.h"

namespace Base
{
    typedef uint8_t State[AMOUNT_OF_SQUARES];
    enum class Type_e : uint8_t;

    class Board
    {
    public:
        Board();
        virtual ~Board() = default;
        /**
         * @brief Start Board class.
         * 
         */
        void Start();
        /**
         * @brief Get state instance of current board.
         * 
         * @param rState
         */
        void GetBoard(State& rState);

    private:
        Device::Square m_squares[AMOUNT_OF_SQUARES];
    };

    enum class Type_e : uint8_t
    {
        NO_PIECE = 0,
        X_PIECE = 1,
        O_PIECE = 2,
    };
}// namespace Base

#endif//ROBOX_BKE_BOARD_H