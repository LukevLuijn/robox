//
// Created by luke on 13-06-22.
//

#include "BKEStrategy.h"
#include <random>

namespace Utils
{
    /*static*/ bool BKEStrategy::m_potentialWin = false;

    int16_t BKEStrategy::CheckBoardLines(BKEPiece_e target, BKEPiece_e source)
    {
        typedef std::array<size_t, 3> Line;
        const std::array<Line, 8> LINES = {
                Line{0, 1, 2},
                Line{3, 4, 5},
                Line{6, 7, 8},
                Line{0, 3, 6},
                Line{1, 4, 7},
                Line{2, 5, 8},
                Line{0, 4, 8},
                Line{6, 4, 2},
        };

        for (const Line& line : LINES)
        {
            auto amount = std::count_if(line.begin(), line.end(), [&target](uint16_t piece) {
                return Driver::BKEDriver::m_board[piece] == (uint16_t) target;
            });
            if (amount >= 2)// almost won
            {
                auto it = std::find_if(line.begin(), line.end(), [&target](uint16_t piece) {
                    return Driver::BKEDriver::m_board[piece] != (uint16_t) target;
                });
                if (it != line.end() &&
                    Driver::BKEDriver::m_board[(*it)] != (uint16_t) source)// piece exists && location is empty.
                {
                    return static_cast<int16_t>(*it);
                }
            }
        }
        return -1;
    }
    int16_t BKEStrategy::CheckMove(BKEPiece_e target)
    {
        auto pieceCount = std::count_if(Driver::BKEDriver::m_board.begin(),
                                        Driver::BKEDriver::m_board.end(),
                                        [&target](uint16_t piece) {
                                            return piece == (uint16_t) target;
                                        });
        switch (pieceCount)
        {
            case 1:
                return CheckMove01(target);
            case 2:
                return CheckMove02(target);
            default:
                return -1;
        }
    }
    bool BKEStrategy::IsBoardFilled()
    {
        auto emptySpaces = std::count_if(Driver::BKEDriver::m_board.begin(),
                                    Driver::BKEDriver::m_board.end(),
                                    [](uint16_t piece) {
                                      return piece == (uint16_t) BKEPiece_e::NO_PIECE;
                                    });
        return emptySpaces == 0;
    }
    int16_t BKEStrategy::CheckMove01(BKEPiece_e target)
    {
        if (CheckCorners(target))
        {
            return m_center;
        }
        else if (CheckCenter(target))
        {
            uint8_t index = GetRandom(0, 4);
            return m_corners[index];
        }
        else if (CheckEdges(target))
        {
            m_potentialWin = true;
            return m_center;
        }
        return -1;// no viable move found
    }
    int16_t BKEStrategy::CheckMove02(BKEPiece_e target)
    {
        if (!m_potentialWin)
        {
            return -1;// no viable move found.
        }
        bool patternVertical = (Driver::BKEDriver::m_board[1] == (uint16_t) target) &&
                               (Driver::BKEDriver::m_board[7] == (uint16_t) target);
        bool patternHorizontal = (Driver::BKEDriver::m_board[3] == (uint16_t) target) &&
                               (Driver::BKEDriver::m_board[5] == (uint16_t) target);

        if (patternVertical || patternHorizontal)
        {
            uint8_t index = GetRandom(0, 4);
            return m_corners[index];
        }
        return -1; // no viable move found.
    }
    bool BKEStrategy::CheckCorners(BKEPiece_e target)
    {
        return Driver::BKEDriver::m_board[0] == (uint16_t) target ||
               Driver::BKEDriver::m_board[2] == (uint16_t) target ||
               Driver::BKEDriver::m_board[6] == (uint16_t) target || Driver::BKEDriver::m_board[8] == (uint16_t) target;
    }
    bool BKEStrategy::CheckEdges(BKEPiece_e target)
    {
        return Driver::BKEDriver::m_board[1] == (uint16_t) target ||
               Driver::BKEDriver::m_board[3] == (uint16_t) target ||
               Driver::BKEDriver::m_board[5] == (uint16_t) target || Driver::BKEDriver::m_board[7] == (uint16_t) target;
    }
    bool BKEStrategy::CheckCenter(BKEPiece_e target)
    {
        return Driver::BKEDriver::m_board[4] == (uint16_t) target;
    }
    uint8_t BKEStrategy::GetRandom(uint8_t min, uint8_t max)
    {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
        return static_cast<uint8_t>(dist(rng));
    }

}// namespace Utils
