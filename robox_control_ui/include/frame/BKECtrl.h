//
// Created by luke on 10-06-22.
//

#ifndef ROBOX_CONTROL_UI_BKECTRL_H
#define ROBOX_CONTROL_UI_BKECTRL_H

#include "wx/statline.h"
#include "wx/stattext.h"
#include "wx/button.h"
#include "wx/bmpbuttn.h"
#include "wx/textctrl.h"
#include "wx/slider.h"

#include "FrameInterface.h"

namespace Frame
{
    class BKECtrl : public IFrame
    {
    public:
        explicit BKECtrl(wxWindow* parent);
        ~BKECtrl() override;

        wxPanel* GetPanel() override;
        void UpdateData(DataType_e responseType) override;
        void ResetPanel() override;

    private:
        void UpdateFields();
        void SyncFields();

    private:
        virtual void OnKillFocusSpeed( wxFocusEvent& event );
        virtual void OnTextEnterSpeed( wxCommandEvent& event );
        virtual void OnTextMaxLenSpeed( wxCommandEvent& event );
        virtual void OnSliderReleaseSpeed( wxScrollEvent& event );
        virtual void OnSliderSpeed( wxCommandEvent& event );
        virtual void OnKillFocusAccel( wxFocusEvent& event );
        virtual void OnTextEnterAccel( wxCommandEvent& event );
        virtual void OnTextMaxLenAccel( wxCommandEvent& event );
        virtual void OnSliderReleaseAccel( wxScrollEvent& event );
        virtual void OnSliderAccel( wxCommandEvent& event );
        virtual void OnClickResetBoard( wxCommandEvent& event );
        virtual void OnClickGoToHome( wxCommandEvent& event );
        virtual void OnClickBoard00( wxCommandEvent& event );
        virtual void OnClickBoard01( wxCommandEvent& event );
        virtual void OnClickBoard02( wxCommandEvent& event );
        virtual void OnClickBoard03( wxCommandEvent& event );
        virtual void OnClickBoard04( wxCommandEvent& event );
        virtual void OnClickBoard05( wxCommandEvent& event );
        virtual void OnClickBoard06( wxCommandEvent& event );
        virtual void OnClickBoard07( wxCommandEvent& event );
        virtual void OnClickBoard08( wxCommandEvent& event );

    private:
        void InitializeFrame();
        void ConnectEvents();
        void DisconnectEvents();
    private:
        std::array<std::pair<float,float>, 2> m_minMaxValues;
    private:
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

        wxBitmapButton* m_buttonBoard0{};
        wxBitmapButton* m_buttonBoard1{};
        wxBitmapButton* m_buttonBoard2{};
        wxBitmapButton* m_buttonBoard3{};
        wxBitmapButton* m_buttonBoard4{};
        wxBitmapButton* m_buttonBoard5{};
        wxBitmapButton* m_buttonBoard6{};
        wxBitmapButton* m_buttonBoard7{};
        wxBitmapButton* m_buttonBoard8{};

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
}

#endif//ROBOX_CONTROL_UI_BKECTRL_H
