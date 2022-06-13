//
// Created by luke on 13-06-22.
//

#ifndef ROBOX_CONTROL_UI_BKESTRATEGY_H
#define ROBOX_CONTROL_UI_BKESTRATEGY_H

#include "BKEDriver.h"

namespace Utils
{

    class BKEStrategy
    {
        typedef Driver::Board Board;
    public:

        static int16_t CheckBoardLines(BKEPiece_e target, BKEPiece_e source);
        static int16_t CheckMove(BKEPiece_e target);
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
