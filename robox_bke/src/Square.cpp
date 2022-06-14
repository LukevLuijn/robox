// ======================================
// Filename:    Square.cpp
// Project:     ROBOX BKE
// Subsystem:   BKE control
// Author:      Luke van Luijn
// Data:        09-06-2022
// ======================================

#include "Square.h"
#include "config.h"

namespace Device
{
    Square::Square() : m_hallPin01(0), m_hallPin02(0)
    {
    }
    Square::Square(uint8_t pin01, uint8_t pin02) : m_hallPin01(pin01), m_hallPin02(pin02)
    {
    }
    void Square::Start()
    {
        pinMode(m_hallPin01, INPUT);
        pinMode(m_hallPin02, INPUT);
    }
    bool Square::HasPiece()
    {
#ifdef DEBUG_MODE
        if (m_hallPin01 == 0 || m_hallPin02 == 0)
        {
            return false;
        }
#endif

        return Triggered01() || Triggered02();
    }
    bool Square::HasPieceX()
    {
        return Triggered01() || Triggered02();
    }
    bool Square::hasPieceO()
    {
        return Triggered01() && Triggered02();
    }
    bool Square::Triggered01()
    {
        return digitalRead(m_hallPin01) == LOW;
    }
    bool Square::Triggered02()
    {
        return digitalRead(m_hallPin02) == LOW;
    }
}// namespace Device