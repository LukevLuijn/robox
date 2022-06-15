// ======================================
// Filename:    BKECtrl.h
// Project:     ROBOX Control UI
// Subsystem:   BKE control
// Author:      Luke van Luijn
// Data:        10-06-2022
// ======================================
#ifndef ROBOX_CONTROL_UI_BKE_CTRL_H
#define ROBOX_CONTROL_UI_BKE_CTRL_H

#include "wx/bmpbuttn.h"
#include "wx/button.h"
#include "wx/tglbtn.h"
#include "wx/slider.h"
#include "wx/statline.h"
#include "wx/stattext.h"
#include "wx/textctrl.h"
#include "wx/timer.h"

#include <chrono>
#include <map>

#include "FrameInterface.h"
#include "bke_locations.h"

wxDEFINE_EVENT(GAME_COMPLETE, wxCommandEvent);

namespace Frame
{
    struct BKELocation {
        float a1, a2, h;
    };
    enum class BKETurn_e : uint8_t
    {
        USER = 0,
        ROBOX = 1,
    };
    enum class StorageType_e : uint8_t
    {
        STORAGE_X = 0,
        STORAGE_O = 1,
        BOARD = 2,
    };
    enum class StorageInteraction_e : uint8_t
    {
        RETRIEVE = 0,
        STORE = 1,
    };
    enum class BKEStrategy_e : uint8_t
    {
        FAIR = 0,
        EASY = 1,
        MANUAL = 2,
    };

    class BKECtrl : public IFrame
    {
    public:
        explicit BKECtrl(wxWindow* parent);
        ~BKECtrl() override;

        /**
         * @brief Get current instance of this panel.
         *
         * @return Current instance of panel.
         */
        wxPanel* GetPanel() override;
        /**
         * @brief Update data based on incoming message.
         *
         * @param responseType Type of data update.
         */
        void UpdateData(DataType_e responseType) override;
        /**
         * @brief Reset panel to initial state.
         *
         * @note Not used.
         */
        void ResetPanel() override;
        /**
         * @brief Update BKE Board.
         *
         * @param type BKE update type.
         */
        void UpdateBKE(BKEType_e type);

        /**
         * @brief End the game.
         */
        void EndGame();

    private:
        void UpdateFields();
        void SyncFields();

        bool OnTextEnter(wxTextCtrl* textCtrl, wxSlider* slider, float& value, Fields_e field);
        void OnSliderMove(wxTextCtrl* textCtrl, wxSlider* slider);

        void PlacePiece(uint8_t index, StorageType_e type, bool home = true);
        void RemovePiece(uint8_t index, StorageType_e type, bool home = true);
        bool GetStorageLocation(StorageType_e type, StorageInteraction_e interaction, BKELocation& location);
        void BoardInteraction(uint8_t index, bool home = true);
        void ClearBoard();
        void UpdateStatistics(BKEResult_e result);
        void SetStatistics();
        void ToggleTurn(BKETurn_e turn);

        static void MoveToActiveHome();
        static void MoveToLocation(const BKELocation& location, bool isTrigger, bool isDrop = false);
        static float GetGripAngle(float A1, float A2);
        static void TriggerGripper(uint8_t value);
        static void Move(float A0, float A1, float A2, float A3);
        static bool PlayPiece(BKEStrategy_e strategy, uint8_t& index);

    private:
        virtual void OnUpdateEvent(wxTimerEvent& event);

        virtual void OnKillFocusSpeed(wxFocusEvent& event);
        virtual void OnTextEnterSpeed(wxCommandEvent& event);
        virtual void OnTextMaxLenSpeed(wxCommandEvent& event);
        virtual void OnSliderReleaseSpeed(wxScrollEvent& event);
        virtual void OnSliderSpeed(wxCommandEvent& event);
        virtual void OnKillFocusAccel(wxFocusEvent& event);
        virtual void OnTextEnterAccel(wxCommandEvent& event);
        virtual void OnTextMaxLenAccel(wxCommandEvent& event);
        virtual void OnSliderReleaseAccel(wxScrollEvent& event);
        virtual void OnSliderAccel(wxCommandEvent& event);
        virtual void OnClickResetBoard(wxCommandEvent& event);
        virtual void OnClickGoToHome(wxCommandEvent& event);
        virtual void OnClickBoard00(wxCommandEvent& event);
        virtual void OnClickBoard01(wxCommandEvent& event);
        virtual void OnClickBoard02(wxCommandEvent& event);
        virtual void OnClickBoard03(wxCommandEvent& event);
        virtual void OnClickBoard04(wxCommandEvent& event);
        virtual void OnClickBoard05(wxCommandEvent& event);
        virtual void OnClickBoard06(wxCommandEvent& event);
        virtual void OnClickBoard07(wxCommandEvent& event);
        virtual void OnClickBoard08(wxCommandEvent& event);

        virtual void OnToggleManual(wxCommandEvent& event);
        virtual void OnToggleFair(wxCommandEvent& event);
        virtual void OnToggleEasy(wxCommandEvent& event);

    private:
        void InitializeFrame();
        void ConnectEvents();
        void DisconnectEvents();

    private:
        std::array<std::pair<float, float>, 2> m_minMaxValues;
        wxBitmap m_bitmapPieceO, m_bitmapPieceX, m_bitmapEmpty;
        std::array<uint8_t, 2> m_storage;

        bool m_gameActive;
        std::chrono::time_point<std::chrono::steady_clock> m_gameStart;
        BKEStrategy_e m_strategy;
        BKETurn_e m_currentTurn;




        const std::array<BKELocation, N_TOTAL_LOCATIONS> m_locations{
                BKELocation{BOARD_LOC_00}, BKELocation{BOARD_LOC_01}, BKELocation{BOARD_LOC_02},
                BKELocation{BOARD_LOC_03}, BKELocation{BOARD_LOC_04}, BKELocation{BOARD_LOC_05},
                BKELocation{BOARD_LOC_06}, BKELocation{BOARD_LOC_07}, BKELocation{BOARD_LOC_08},
                BKELocation{STORAGE_X_00}, BKELocation{STORAGE_X_01}, BKELocation{STORAGE_X_02},
                BKELocation{STORAGE_X_03}, BKELocation{STORAGE_X_04}, BKELocation{STORAGE_O_00},
                BKELocation{STORAGE_O_01}, BKELocation{STORAGE_O_02}, BKELocation{STORAGE_O_03},
                BKELocation{STORAGE_O_04},
        };

    private:
        std::array<wxBitmapButton*, 9> m_boardButtons{};

        wxTimer* m_timer;


        wxPanel* m_speedPanel{};
        wxPanel* m_boardPanel{};
        wxPanel* m_panel39{};
        wxPanel* m_panel41{};
        wxPanel* m_turnPanel{};
        wxPanel* m_panel42{};
        wxPanel* m_panel43{};
        wxPanel* m_statsPanel{};

        wxTextCtrl* m_fieldSpeed{};
        wxTextCtrl* m_fieldAccel{};
        wxSlider* m_sliderSpeed{};
        wxSlider* m_sliderAccel{};

        wxStaticLine* m_staticLine21{};
        wxStaticLine* m_staticLine25{};
        wxStaticLine* m_staticLine24{};
        wxStaticLine* m_staticLine22{};
        wxStaticLine* m_staticLine23{};
        wxStaticLine* m_staticLine28{};

        wxButton* m_buttonResetBoard{};
        wxButton* m_buttonHome{};

        wxToggleButton* m_strategyButtonManual{};
        wxToggleButton* m_strategyButtonFair{};
        wxToggleButton* m_strategyButtonEasy{};

        wxStaticText* m_textSpeed{};
        wxStaticText* m_iconSpeed{};
        wxStaticText* m_textAccel{};
        wxStaticText* m_iconAccel{};
        wxStaticText* m_textTurn{};
        wxStaticText* m_textTurnROBOX{};
        wxStaticText* m_textTurnUser{};
        wxStaticText* m_textRunTime{};
        wxStaticText* m_textRunTimeValue{};

        wxStaticText* m_textGameTime{};
        wxStaticText* m_textGameTimeValue{};

        wxStaticText* m_textGamesPlayed{};
        wxStaticText* m_textGamesPlayedValue{};
        wxStaticText* m_textWon{};
        wxStaticText* m_textWonValue{};
        wxStaticText* m_iconWon0{};
        wxStaticText* m_textWonPercentage{};
        wxStaticText* m_IconWon1{};
        wxStaticText* m_textLost{};
        wxStaticText* m_textLostValue{};
        wxStaticText* m_iconLost0{};
        wxStaticText* m_textLostPercentage{};
        wxStaticText* m_iconLost1{};
        wxStaticText* m_textDraw{};
        wxStaticText* m_textDrawValue{};
        wxStaticText* m_iconDraw0{};
        wxStaticText* m_textDrawPercentage{};
        wxStaticText* m_iconDraw1{};
    };
}// namespace Frame

#endif//ROBOX_CONTROL_UI_BKE_CTRL_H
