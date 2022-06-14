// ======================================
// Filename:    BKEStrategy.h
// Project:     ROBOX Control UI
// Subsystem:   BKE control
// Author:      Luke van Luijn
// Data:        13-06-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_BKESTRATEGY_H
#define ROBOX_CONTROL_UI_BKESTRATEGY_H

#include "BKEDriver.h"

namespace Utils
{

    class BKEStrategy
    {
        typedef Driver::Board Board;
    public:
        /**
         * @brief Check all possible win/lose scenario's. compute adequate response.
         *
         * @param target Target piece type.
         * @param source Source piece type.
         * @return Index of next move. returns -1 if there are no possible defensive/offensive moves.
         */
        static int16_t CheckBoardLines(BKEPiece_e target, BKEPiece_e source);
        /**
         * @brief Check general moves and compute response.
         *
         * @param target Target piece type.
         * @return Index of next move. returns -1 if no valid move was found.
         */
        static int16_t CheckMove(BKEPiece_e target);
        /**
         * @brief Check if all places on the board are filled with pieces.
         *
         * @return True if all nine spaces are filled. False if not.
         */
        static bool IsBoardFilled();

    private:
        static int16_t CheckMove01(BKEPiece_e target);
        static int16_t CheckMove02(BKEPiece_e target);

        static bool CheckCorners(BKEPiece_e target);
        static bool CheckEdges(BKEPiece_e target);
        static bool CheckCenter(BKEPiece_e target);

        static uint8_t GetRandom(uint8_t min, uint8_t max);

    private:
        static bool m_potentialWin;

        static constexpr std::array<uint8_t, 4> m_corners = {0,2,6,8};
        static constexpr std::array<uint8_t, 4> m_edges = {1,3,5,7};
        static constexpr uint8_t m_center = 4;
    };
}

#endif//ROBOX_CONTROL_UI_BKESTRATEGY_H
