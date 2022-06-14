// ======================================
// Filename:    Square.h
// Project:     ROBOX BKE
// Subsystem:   BKE control
// Author:      Luke van Luijn
// Data:        09-06-2022
// ======================================

#ifndef ROBOX_BKE_SQUARE_H
#define ROBOX_BKE_SQUARE_H

#include <Arduino.h>

namespace Device
{
    class Square
    {
    public:
        Square();
        Square(uint8_t pin01, uint8_t pin02);
        virtual ~Square() = default;

        /**
         * @brief Start square instance.
         * 
         */
        void Start();
        /**
         * @brief Check if square contains piece.
         * 
         * @return true If square contains a piece.
         * @return false If square does not contain a piece.
         */
        bool HasPiece();
        /**
         * @brief Check if square contains X-piece.
         * 
         * @return true If square contains an X-piece.
         * @return false If square does not contain an X-piece.
         */
        bool HasPieceX();
        /**
         * @brief Check if square contains O-piece.
         * 
         * @return true If square contains an O-piece.
         * @return false If square does not contain an O-piece.
         */
        bool hasPieceO();

    private:
        bool Triggered01();
        bool Triggered02();

    private:
        uint8_t m_hallPin01, m_hallPin02;
    };
}// namespace Device

#endif// ROBOX_BKE_SQUARE_H