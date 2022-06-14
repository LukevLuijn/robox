// ======================================
// Filename:    Game.cpp
// Project:     ROBOX BKE
// Subsystem:   BKE control
// Author:      Luke van Luijn
// Data:        09-06-2022
// ======================================

#include <Entropy.h>

#include "Game.h"
#include "sounds.h"

namespace Base
{
    Game::Game() : m_timer(0), m_newPiece(false), m_lastSoundIndex(0)
    {
    }
    bool Game::Start()
    {
        Entropy.Initialize();
        m_board.Start();
        m_board.GetBoard(m_previousState);

        Serial2.begin(9600);
        delay(1000);

        uint8_t counter = 0;
        while (!m_player.begin(Serial2))
        {
#ifdef DEBUG_MODE
            Serial.print("Could not connect to player");
            Serial.print("try: ");
            Serial.println(counter);
#endif
            if (counter > 10)
            {
#ifdef DEBUG_MODE
                Serial.println("Max amount of tries reached");
#endif
                return false;
            }
        }

        return true;
    }
    void Game::Run()
    {
        State current;
        m_board.GetBoard(current);

        if (IsBoardEmpty() && (m_gameComplete || m_gameMessagePlayed))
        {
            // board empty, game reset.
            m_gameComplete = false;
            m_gameMessagePlayed = false;
        }

        if (IsNewState(current, m_previousState) && !m_newPiece)
        {
            m_newPiece = true;
            m_timer = millis();
        }

        if (m_newPiece && Timer(m_timer, INTERVAL))
        {
            m_gameComplete = CheckState(current);
            SendMessage(current, m_gameComplete);
            SetPreviousState(current);
            m_newPiece = false;

            if (m_gameComplete)
            {
                delay(100);
                EndMessage(m_gameResult);
            }
        }
    }
    void Game::SendMessage(State state, bool gameComplete)
    {
        String header = MESSAGE_HEADER;
        String type = (gameComplete) ? DATA_INDEX : UPDATE_INDEX;

        String body = "[";

        for (size_t i = 0; i < AMOUNT_OF_SQUARES; ++i)
        {
            body.append((uint16_t) state[i]);
            body.append((i != AMOUNT_OF_SQUARES - 1) ? ',' : ']');
        }

        String message = header + type + body;
        WriteMessage(message);
    }
    bool Game::IsNewState(State A, State B)
    {
        for (size_t i = 0; i < AMOUNT_OF_SQUARES; ++i)
        {
            if (A[i] != B[i])
            {
                return true;
            }
        }
        return false;
    }
    void Game::SetPreviousState(State newState)
    {
        for (size_t i = 0; i < AMOUNT_OF_SQUARES; ++i)
        {
            m_previousState[i] = newState[i];
        }
    }
    bool Game::CheckState(State state)
    {
        static constexpr uint8_t nCols = 3;
        static constexpr uint8_t nRows = 3;

        uint8_t layout[nCols][nRows] = {
                {state[0], state[1], state[2]},
                {state[3], state[4], state[5]},
                {state[6], state[7], state[8]},
        };

#ifdef DEBUG_MODE
        PrintBoard(layout);
#endif

        m_gameResult = CheckCols(layout);
        if (m_gameResult != Type_e::NO_PIECE)
        {
            // EndMessage(m_gameResult);
            return true;
        }
        m_gameResult = CheckRows(layout);
        if (m_gameResult != Type_e::NO_PIECE)
        {
            // EndMessage(m_gameResult);
            return true;
        }
        m_gameResult = CheckDiagonal(layout);
        if (m_gameResult != Type_e::NO_PIECE)
        {
            // EndMessage(m_gameResult);
            return true;
        }

        bool fullBoard = true;
        for (size_t i = 0; i < AMOUNT_OF_SQUARES; ++i)
        {
            if (state[i] == (uint8_t) Type_e::NO_PIECE)
            {
                fullBoard = false;
                break;
            }
        }
        if (fullBoard)
        {
            m_gameResult = Type_e::NO_PIECE;
            return true;
            // DrawMessage();
        }
        return false;
    }
    Type_e Game::CheckCols(Layout board)
    {
        for (size_t col = 0; col < N_COLS; ++col)
        {
            uint8_t xCounter = 0, oCounter = 0;
            for (size_t row = 0; row < N_ROWS; ++row)
            {
                Type_e piece = (Type_e) board[col][row];
                oCounter += (piece == Type_e::O_PIECE) ? 1 : 0;
                xCounter += (piece == Type_e::X_PIECE) ? 1 : 0;
            }

            if (xCounter == N_COLS)
            {
                return Type_e::X_PIECE;
            }
            else if (oCounter == N_COLS)
            {
                return Type_e::O_PIECE;
            }
        }
        return Type_e::NO_PIECE;
    }
    Type_e Game::CheckRows(Layout board)
    {
        for (size_t row = 0; row < N_ROWS; ++row)
        {
            uint8_t xCounter = 0, oCounter = 0;
            for (size_t col = 0; col < N_COLS; ++col)
            {
                Type_e piece = (Type_e) board[col][row];
                oCounter += (piece == Type_e::O_PIECE) ? 1 : 0;
                xCounter += (piece == Type_e::X_PIECE) ? 1 : 0;
            }

            if (xCounter == N_COLS)
            {
                return Type_e::X_PIECE;
            }
            else if (oCounter == N_COLS)
            {
                return Type_e::O_PIECE;
            }
        }
        return Type_e::NO_PIECE;
    }
    Type_e Game::CheckDiagonal(Layout board)
    {
        Type_e TL = (Type_e) board[0][0];
        Type_e TR = (Type_e) board[0][2];
        Type_e MM = (Type_e) board[1][1];
        Type_e BL = (Type_e) board[2][0];
        Type_e BR = (Type_e) board[2][2];

        if ((TL == Type_e::O_PIECE && MM == Type_e::O_PIECE && BR == Type_e::O_PIECE) ||
            (TR == Type_e::O_PIECE && MM == Type_e::O_PIECE && BL == Type_e::O_PIECE))
        {
            return Type_e::O_PIECE;
        }
        else if ((TL == Type_e::X_PIECE && MM == Type_e::X_PIECE && BR == Type_e::X_PIECE) ||
                 (TR == Type_e::X_PIECE && MM == Type_e::X_PIECE && BL == Type_e::X_PIECE))
        {
            return Type_e::X_PIECE;
        }
        return Type_e::NO_PIECE;
    }
    void Game::EndMessage(Type_e type)
    {
        if (type == Type_e::O_PIECE)
        {
            WonMessage();
        }
        else if (type == Type_e::X_PIECE)
        {
            LostMessage();
        }
        else
        {
            DrawMessage();
        }
    }
    void Game::LostMessage()
    {
        String header = MESSAGE_HEADER + String((uint16_t) RESULT_INDEX);
        String body = "[" + String((uint16_t) Results_e::GAME_LOST) + "]";
        String message = header + body;

        WriteMessage(message);
        PlaySound(LOST_START_INDEX, LOST_AMOUNT_OF_SOUNDS);

#ifdef DEBUG_MODE
        Serial.println("Game: Lost");
#endif
    }
    void Game::WonMessage()
    {
        String header = MESSAGE_HEADER + String((uint16_t) RESULT_INDEX);
        String body = "[" + String((uint16_t) Results_e::GAME_WON) + "]";
        String message = header + body;

        WriteMessage(message);
        PlaySound(WON_START_INDEX, WON_AMOUNT_OF_SOUNDS);

#ifdef DEBUG_MODE
        Serial.println("Game: Won");
#endif
    }
    void Game::DrawMessage()
    {
        String header = MESSAGE_HEADER + String((uint16_t) RESULT_INDEX);
        String body = "[" + String((uint16_t) Results_e::GAME_DRAW) + "]";
        String message = header + body;

        WriteMessage(message);
        PlaySound(DRAW_START_INDEX, DRAW_AMOUNT_OF_SOUNDS);

#ifdef DEBUG_MODE
        Serial.println("Game: Draw");
#endif
    }
    void Game::WriteMessage(String text)
    {
        String message = MESSAGE_START_CHAR + text + MESSAGE_STOP_CHAR;
        Serial.write(message.c_str());
        Serial.send_now();
        delay(5);
    }
    /*static*/ bool Game::Timer(uint32_t& time, uint32_t interval)
    {
        if (millis() - time >= interval)
        {
            time = millis();
            return true;
        }
        return false;
    }
    void Game::PlaySound(uint8_t start, uint8_t size)
    {
        if (!m_gameMessagePlayed)
        {
            uint8_t soundIndex = m_lastSoundIndex;
            while (soundIndex == m_lastSoundIndex)
            {
                soundIndex = Entropy.random(0, size) + start;
            }

            m_player.play(soundIndex);
            m_lastSoundIndex = soundIndex;
            m_gameMessagePlayed = true;

#ifdef DEBUG_MODE
            Serial.print("Sound index: ");
            Serial.println(soundIndex);
#endif
        }
    }
    void Game::PrintBoard(Layout board)
    {
        Serial.println();
        for (int16_t i = N_ROWS - 1; i >= 0; --i)
        {
            for (int16_t j = N_COLS - 1; j >= 0; --j)
            {
                String value = (board[i][j] == (uint8_t) Type_e::NO_PIECE)
                                       ? "."
                                       : ((board[i][j] == (uint8_t) Type_e::O_PIECE ? "O" : "X"));
                Serial.print(value);
                Serial.print(" ");
            }
            Serial.println();
        }
    }
    bool Game::IsBoardEmpty()
    {
        State currentState;
        m_board.GetBoard(currentState);

        for (uint8_t sqr : currentState)
        {
            if (sqr != (uint8_t) Type_e::NO_PIECE)
            {
                return false;
            }
        }
        return true;
    }


}// namespace Base