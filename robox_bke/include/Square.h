
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

        void Start();

        bool HasPiece();
        bool HasPieceX();
        bool hasPieceO();

    private:
        bool Triggered01();
        bool Triggered02();

    private:
        uint8_t m_hallPin01, m_hallPin02;
    };
}// namespace Device

#endif// ROBOX_BKE_SQUARE_H