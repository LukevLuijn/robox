//
// Created by luke on 21-05-22.
//

#ifndef ROBOT_VISUAL_MANUALCTRL_H
#define ROBOT_VISUAL_MANUALCTRL_H

#include "wx/wx.h"
#include "wx/tglbtn.h"
#include "wx/statline.h"

#include "RobotDrawPanel.h"
#include "FrameInterface.h"

namespace Frame
{
    enum class Fields_e : uint8_t;

    class ManualCtrl : public IFrame
    {
    public:
        explicit ManualCtrl(wxWindow* parent);
        ~ManualCtrl() override;

        /**
         * @brief Get the panel of the frame.
         * 
         * @return wxPanel* 
         */
        wxPanel* GetPanel() override;
        /**
         * @brief Update frame data.
         * 
         * @param responseType 
         */
        void UpdateData(DataType_e responseType) override;
        /**
         * @brief Reset frame panel.
         * 
         */
        void ResetPanel() override;

    private:
        void UpdateFields();
        void SyncFields();
        bool OnTextEnter(wxTextCtrl* textCtrl, wxSlider* slider, float& value, Fields_e field);
        static void OnSliderMove(wxTextCtrl* textCtrl, wxSlider* slider);
        void UpdateDrawPanel();

        void OnKillFocusSpeed(wxFocusEvent& event);
        void OnKillFocusAccel(wxFocusEvent& event);
        void OnKillFocusZ(wxFocusEvent& event);
        void OnKillFocusA1(wxFocusEvent& event);
        void OnKillFocusA2(wxFocusEvent& event);
        void OnKillFocusA3(wxFocusEvent& event);

        void OnTextEnterSpeed(wxCommandEvent& event);
        void OnTextEnterAccel(wxCommandEvent& event);
        void OnTextEnterZ(wxCommandEvent& event);
        void OnTextEnterA1(wxCommandEvent& event);
        void OnTextEnterA2(wxCommandEvent& event);
        void OnTextEnterA3(wxCommandEvent& event);

        void OnSliderReleaseSpeed(wxScrollEvent& event);
        void OnSliderReleaseAccel(wxScrollEvent& event);
        void OnSliderReleaseZ(wxScrollEvent& event);
        void OnSliderReleaseA1(wxScrollEvent& event);
        void OnSliderReleaseA2(wxScrollEvent& event);
        void OnSliderReleaseA3(wxScrollEvent& event);

        void OnSliderSpeed(wxCommandEvent& event);
        void OnSliderAccel(wxCommandEvent& event);
        void OnSliderZ(wxCommandEvent& event);
        void OnSliderA1(wxCommandEvent& event);
        void OnSliderA2(wxCommandEvent& event);
        void OnSliderA3(wxCommandEvent& event);

        void OnToggleGrip01(wxCommandEvent& event);
        void OnToggleGrip02(wxCommandEvent& event);
        void OnToggleAuto(wxCommandEvent& event);
        void OnMouseRelease(wxMouseEvent& event);
        void OnMouseMotion(wxMouseEvent& event);

        void OnTextMaxLenSpeed(wxCommandEvent& event);
        void OnTextMaxLenAccel(wxCommandEvent& event);
        void OnTextMaxLenZ(wxCommandEvent& event);
        void OnTextMaxLenA1(wxCommandEvent& event);
        void OnTextMaxLenA2(wxCommandEvent& event);
        void OnTextMaxLenA3(wxCommandEvent& event);

        void InitializeFrame();
        void ConnectEvents();
        void DisconnectEvents();
    private:
        std::array<std::pair<float,float>, 6> m_minMaxValues;
        bool m_updateFromDrawPanel;

    private:
        RobotDrawPanel* m_drawPanel{};

        wxPanel* m_configPanel{};
        wxPanel* m_togglePanel{};
        wxPanel* m_positionPanel{};

        wxStaticText* m_textSpeed{};
        wxStaticText* m_textAccel{};
        wxStaticText* m_textZ{};
        wxStaticText* m_textA1{};
        wxStaticText* m_textA2{};
        wxStaticText* m_textA3{};

        wxStaticText* m_iconSpeed{};
        wxStaticText* m_iconAccel{};
        wxStaticText* m_iconZ{};
        wxStaticText* m_iconA1{};
        wxStaticText* m_iconA2{};
        wxStaticText* m_iconA3{};

        wxTextCtrl* m_textCtrlSpeed{};
        wxTextCtrl* m_textCtrlAccel{};
        wxTextCtrl* m_textCtrlZ{};
        wxTextCtrl* m_textCtrlA1{};
        wxTextCtrl* m_textCtrlA2{};
        wxTextCtrl* m_textCtrlA3{};

        wxSlider* m_sliderSpeed{};
        wxSlider* m_sliderAccel{};
        wxSlider* m_sliderZ{};
        wxSlider* m_sliderA1{};
        wxSlider* m_sliderA2{};
        wxSlider* m_sliderA3{};

        wxToggleButton* m_toggleGrip01{};
        wxToggleButton* m_toggleGrip02{};
        wxToggleButton* m_toggleAuto{};

        wxStaticLine* m_line0{};
        wxStaticLine* m_line1{};
        wxStaticLine* m_line2{};
    };

    enum class Fields_e : uint8_t
    {
        FIELD_SPEED = 0,
        FIELD_ACCEL = 1,
        FIELD_Z = 2,
        FIELD_A1 = 3,
        FIELD_A2 = 4,
        FIELD_A3 = 5,
    };
}// namespace Frame

#endif//ROBOT_VISUAL_MANUALCTRL_H
