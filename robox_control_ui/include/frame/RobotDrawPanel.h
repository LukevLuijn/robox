// ======================================
// Filename:    RobotDrawPanel.h
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        20-05-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_ROBOT_DRAW_PANEL_H
#define ROBOX_CONTROL_UI_ROBOT_DRAW_PANEL_H

#include "DrawPanel.h"
#include "Kinematics.h"

#include "wx/stattext.h"

namespace Frame
{
    class Overlay : public wxPanel
    {
    public:
        explicit Overlay(wxWindow* parent)
            : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL)
        {
            InitializeFrame();
        }
        ~Overlay() override = default;

        void SetX(float value);
        void SetY(float value);
        void SetFields(float fieldX, float fieldY);
        [[nodiscard]] float GetX() const;
        [[nodiscard]] float GetY() const;

        void UpdateOverlay();

    private:
        void InitializeFrame();

    private:
        wxStaticText* m_textX{};
        wxStaticText* m_textValueX{};
        wxStaticText* m_iconX{};
        wxStaticText* m_textY{};
        wxStaticText* m_textValueY{};
        wxStaticText* m_iconY{};
    };

    struct Joint {
        double x, y;
    };

    struct Chain {
        wxPoint base, mid, end;
        double angle0{}, angle1{};
    };

    class RobotDrawPanel : public Widgets::DrawPanel
    {
    private:
    public:
        RobotDrawPanel(wxWindow* parent, int32_t* sizes);
        ~RobotDrawPanel() override = default;

        /**
         * @brief Update panel data.
         * 
         */
        void UpdateData();
        /**
         * @brief Set new angles for the preview.
         * 
         * @param angles New angles in degrees.
         */
        void SetAngles(const std::array<double, 2>& angles);
        /**
         * @brief Get the current angles of the preview.
         * 
         * @return std::array<double, 2> Array of angles in degrees.
         */
        std::array<double, 2> GetAngles();
        /**
         * @brief Get the current position of the last joint (A2).
         * 
         * @return wxPoint Position of the joint.
         */
        wxPoint GetPosition();

    private:
        void Start();
        static float GetDistance(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
        static float GetAngle(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
        void UpdateJoints(Chain& chain, Utils::Kinematics& IK);
        void UpdateAngles(std::array<double, 2> angles);
        void UpdateOverlay();

    private:
        void Render(wxDC& dc) override;

        void RenderRobot(wxDC& dc, const Chain& chain, const wxColor& color, const wxColor& selected);

        void OnLeftDown(wxMouseEvent& event) override;
        void OnLeftUp(wxMouseEvent& event) override;
        void OnMotion(wxMouseEvent& event) override;

        void InitializeFrame();

    private:
        bool m_initialized, m_selected;

        Overlay* m_overlayPanel{};

        std::array<double, 2> m_sizes{};
        std::array<int32_t, 2> m_offset{};

        Utils::Kinematics m_previewIK;
        Utils::Kinematics m_shadowIK;
        Chain m_previewChain;
        Chain m_shadowChain;

        std::array<float, 2> m_currentPosition{};
        std::array<float, 2> m_positionBuffer{};

        const wxColor BACKGROUND = {80, 80, 80};
        const wxColor SELECTED = {255, 0, 0};
        const wxColor DEFAULT = {255, 255, 255};
        const wxColor SHADOW = {100, 100, 100};
        const wxColor ACCENT = {0, 0, 0};

        double m_staticOffset{};
    };


}// namespace Frame

#endif//ROBOX_CONTROL_UI_ROBOT_DRAW_PANEL_H
