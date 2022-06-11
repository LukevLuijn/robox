//
// Created by luke on 10-06-22.
//

#ifndef ROBOX_CONTROL_UI_BKECTRL_H
#define ROBOX_CONTROL_UI_BKECTRL_H

#include "wx/bmpbuttn.h"
#include "wx/button.h"
#include "wx/slider.h"
#include "wx/statline.h"
#include "wx/stattext.h"
#include "wx/textctrl.h"

#include "FrameInterface.h"
#include "bke_locations.h"

namespace Frame
{
    struct BKELocation {
        float a1, a2, h;
    };

    enum class StorageType_e : uint8_t;
    enum class StorageInteraction_e : uint8_t;

    class BKECtrl : public IFrame
    {
    public:
        explicit BKECtrl(wxWindow* parent);
        ~BKECtrl() override;

        wxPanel* GetPanel() override;
        void UpdateData(DataType_e responseType) override;
        void ResetPanel() override;

        void UpdateBKE(BKEType_e type);

    private:
        void UpdateFields();
        void SyncFields();

        bool OnTextEnter(wxTextCtrl* textCtrl, wxSlider* slider, float& value, Fields_e field);
        void OnSliderMove(wxTextCtrl* textCtrl, wxSlider* slider);

        void PlacePiece(uint8_t index, StorageType_e type, bool home = true);
        void RemovePiece(uint8_t index, StorageType_e type, bool home = true);
        bool GetStorageLocation(StorageType_e type, StorageInteraction_e interaction, BKELocation& location);
        void BoardInteraction(uint8_t index, bool home = true);

        static void MoveToLocation(const BKELocation& location, bool isTrigger, bool isDrop = false);
        static void MoveToActiveHome();
        static void TriggerGripper(uint8_t value);
        static void Move(float A0, float A1, float A2, float A3);
    private:
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

    private:
        void InitializeFrame();
        void ConnectEvents();
        void DisconnectEvents();

    private:
        std::array<std::pair<float, float>, 2> m_minMaxValues;
        wxBitmap m_bitmapPieceO, m_bitmapPieceX, m_bitmapEmpty;
        std::array<uint8_t, 2> m_storage;

        std::array<BKELocation, N_TOTAL_LOCATIONS> m_locations{
                BKELocation{BOARD_LOC_00}, BKELocation{BOARD_LOC_01}, BKELocation{BOARD_LOC_02},
                BKELocation{BOARD_LOC_03}, BKELocation{BOARD_LOC_04}, BKELocation{BOARD_LOC_05},
                BKELocation{BOARD_LOC_06}, BKELocation{BOARD_LOC_07}, BKELocation{BOARD_LOC_08},
                BKELocation{STORAGE_X_00}, BKELocation{STORAGE_X_01}, BKELocation{STORAGE_X_02},
                BKELocation{STORAGE_X_03}, BKELocation{STORAGE_X_04}, BKELocation{STORAGE_O_00},
                BKELocation{STORAGE_O_01}, BKELocation{STORAGE_O_02}, BKELocation{STORAGE_O_03},
                BKELocation{STORAGE_O_04},
        };

    private:
        std::array<wxBitmapButton*, 9> m_boardButtons;

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

        wxStaticText* m_textSpeed{};
        wxStaticText* m_iconSpeed{};
        wxStaticText* m_textAccel{};
        wxStaticText* m_iconAccel{};
        wxStaticText* m_textTurn{};
        wxStaticText* m_textTurnRobox{};
        wxStaticText* m_textTurnUser{};
        wxStaticText* m_textRunTime{};
        wxStaticText* m_textRunTimeValue{};
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

    enum class StorageType_e : uint8_t
    {
        STORAGE_X = 0,
        STORAGE_O = 1,
    };
    enum class StorageInteraction_e : uint8_t
    {
        RETRIEVE = 0,
        STORE = 1,
    };
}// namespace Frame

#endif//ROBOX_CONTROL_UI_BKECTRL_H
