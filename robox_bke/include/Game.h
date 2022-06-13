#ifndef ROBOX_BKE_GAME_H
#define ROBOX_BKE_GAME_H

#include <DFRobotDFPlayerMini.h>

#include "Board.h"

namespace Base
{
    typedef uint8_t Layout [N_ROWS][N_COLS];

    class Game
    {
    public:
        Game();
        virtual ~Game() = default;

        bool Start();
        void Run();

    private:
        void SendMessage(State state, bool gameComplete = false);
        bool IsNewState(State A, State B);
        void SetPreviousState(State newState);
        bool CheckState(State state);

        Type_e CheckCols(Layout board);
        Type_e CheckRows(Layout board);
        Type_e CheckDiagonal(Layout board);

        void EndMessage(Type_e type);
        void LostMessage();
        void WonMessage();
        void DrawMessage();

        void WriteMessage(String text);

        static bool Timer(uint32_t& time, uint32_t interval);
        void PlaySound(uint8_t start, uint8_t size);
        void PrintBoard(Layout board);
        bool IsBoardEmpty();

    private:
        const uint32_t INTERVAL = 2500;

        Board m_board;
        State m_previousState;
        DFRobotDFPlayerMini m_player;

        uint32_t m_timer;
        bool m_newPiece;
        uint8_t m_lastSoundIndex;
        bool m_gameComplete;
        bool m_gameMessagePlayed;
        Type_e m_gameResult;
    };

    enum class Results_e :uint8_t 
    {
        GAME_WON = 0,
        GAME_LOST = 1,
        GAME_DRAW = 2,
    };
}// namespace Base

#endif// ROBOX_BKE_GAME_H