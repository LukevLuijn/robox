// ======================================
// Filename:    RobotDrawPanel.cpp
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        20-05-2022
// ======================================

#include "RobotDrawPanel.h"
#include "RobotDriver.h"
#include "String.h"

#include "wx/dc.h"
#include "wx/sizer.h"

#define TO_RADIANS(a) (a * M_PI / 180)// convert radians to degrees
#define TO_DEGREES(a) (a * 180 / M_PI)// convert degrees to radians

namespace Frame
{
    /** ========================================================== **/
    /** ROBOT DRAW PANEL--------------------------ROBOT DRAW PANEL **/
    /** ========================================================== **/

    RobotDrawPanel::RobotDrawPanel(wxWindow* parent, int32_t* sizes)
        : DrawPanel(parent), m_initialized(false), m_selected(false), m_previewIK(3, sizes), m_shadowIK(3, sizes)
    {
        InitializeFrame();

        m_sizes[0] = static_cast<float>(sizes[0]) * 0.5f;
        m_sizes[1] = static_cast<float>(sizes[1]) * 0.5f;

        m_currentPosition[0] = static_cast<float>(TO_RADIANS(-90));
        m_currentPosition[1] = 0;

        m_previewIK.SetTolerance(0.01f);
        m_shadowIK.SetTolerance(0.01f);

        float rAngles[] = {0, 0, 0};
        int32_t scaledSizes[] = {static_cast<int32_t>(m_sizes[0]), static_cast<int32_t>(m_sizes[1])};

        m_previewIK.SetJoints(rAngles, scaledSizes);
        UpdateJoints(m_previewChain, m_previewIK);

        m_shadowIK.SetJoints(rAngles, scaledSizes);
        UpdateJoints(m_shadowChain, m_shadowIK);
    }
    void RobotDrawPanel::UpdateData()
    {
        float angle0 = -Driver::RobotDriver::m_dataManager.m_segment01.m_currentPosition; // flip angles
        float angle1 = -Driver::RobotDriver::m_dataManager.m_segment02.m_currentPosition; // flip angles
        angle0 += 90; // add offset

        float rAngles[] = {
                m_shadowIK.GetAngle(0),
                static_cast<float>(TO_RADIANS(angle0)),
                static_cast<float>(TO_RADIANS(angle1)),
        };

        int32_t sizes[] = {static_cast<int32_t>(m_sizes[0]), static_cast<int32_t>(m_sizes[1])};

        // ignore updates when there is nothing to update..
        if (rAngles[1] != m_shadowIK.GetAngle(1) || rAngles[2] != m_shadowIK.GetAngle(2))
        {
            m_shadowIK.SetJoints(rAngles, sizes);
            UpdateJoints(m_shadowChain, m_shadowIK);
            PainNow();
        }
    }
    void RobotDrawPanel::SetAngles(const std::array<double, 2>& angles)
    {
        float rAngles[] = {
                m_previewIK.GetAngle(0),
                static_cast<float>(TO_RADIANS(-angles[0]) - m_currentPosition[0] + TO_RADIANS(m_staticOffset)),
                static_cast<float>(TO_RADIANS(-angles[1])),
        };

        m_positionBuffer[0] = rAngles[1];
        m_positionBuffer[1] = rAngles[2];

        int32_t sizes[] = {static_cast<int32_t>(m_sizes[0]), static_cast<int32_t>(m_sizes[1])};
        m_previewIK.SetJoints(rAngles, sizes);


        UpdateJoints(m_previewChain, m_previewIK);
        UpdateOverlay();
        PainNow();
    }
    std::array<double, 2> RobotDrawPanel::GetAngles()
    {
        auto angle0 = static_cast<float>(TO_DEGREES(-m_positionBuffer[0])); // flip angles
        auto angle1 = static_cast<float>(TO_DEGREES(-m_positionBuffer[1])); // flip angles

        std::array<double, 2> array = {angle0,angle1};
        return array;
    }
    wxPoint RobotDrawPanel::GetPosition()
    {
        auto pY = static_cast<int32_t>(m_previewIK.GetY(2) / 0.5f);
        auto pX = static_cast<int32_t>(m_previewIK.GetX(2) / 0.5f);
        return wxPoint(pX, pY);
    }
    void RobotDrawPanel::Start()
    {
        if (!m_initialized)
        {
            SetBackgroundColour(BACKGROUND);

            m_offset[0] = (GetSize().x) / 2;
            m_offset[1] = static_cast<int32_t>(m_sizes[1] * 1.2);

            m_previewChain.base.x = m_offset[0];
            m_previewChain.base.y = m_offset[1];
            m_previewChain.mid.x = m_offset[0];
            m_previewChain.mid.y = static_cast<int32_t>(m_offset[1] + m_sizes[0]);
            m_previewChain.end.x = m_offset[0];
            m_previewChain.end.y = static_cast<int32_t>(m_offset[1] + (m_sizes[0] + m_sizes[1]));

            m_shadowChain = m_previewChain;

            float angles[] = {0, 0};
            int32_t sizes[] = {static_cast<int32_t>(m_sizes[0]), static_cast<int32_t>(m_sizes[1])};
            m_previewIK.SetJoints(angles, sizes);
            m_shadowIK.SetJoints(angles, sizes);

            m_initialized = true;
        }
    }
    float RobotDrawPanel::GetDistance(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
    {
        auto val1 = static_cast<float>(x1 - x2);
        auto val2 = static_cast<float>(y1 - y2);

        return std::sqrt((val1 * val1) + (val2 * val2));
    }
    float RobotDrawPanel::GetAngle(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
    {
        double deltaY = (y2 - y1);
        double deltaX = (x2 - x1);
        double radians = atan2(deltaY, deltaX);

        return static_cast<float>(radians);
    }
    void RobotDrawPanel::UpdateJoints(Chain& chain, Utils::Kinematics& IK)
    {
        chain.base.x = static_cast<int32_t>(IK.GetX(0)) + m_offset[0];
        chain.base.y = static_cast<int32_t>(IK.GetY(0)) + m_offset[1];
        chain.mid.x = static_cast<int32_t>(IK.GetX(1)) + m_offset[0];
        chain.mid.y = static_cast<int32_t>(IK.GetY(1)) + m_offset[1];
        chain.end.x = static_cast<int32_t>(IK.GetX(2)) + m_offset[0];
        chain.end.y = static_cast<int32_t>(IK.GetY(2)) + m_offset[1];
    }
    void RobotDrawPanel::UpdateAngles(std::array<double, 2> angles)
    {
        float rAngles[] = {
                static_cast<float>(TO_RADIANS(angles[0])),
                static_cast<float>(TO_RADIANS(angles[0])),
                static_cast<float>(TO_RADIANS(angles[1])),
        };
        int32_t sizes[] = {static_cast<int32_t>(m_sizes[0]), static_cast<int32_t>(m_sizes[1])};
        m_previewIK.SetJoints(rAngles, sizes);
    }
    void RobotDrawPanel::UpdateOverlay()
    {
        const float fieldX = m_previewIK.GetX(2) / 0.5f;
        const float fieldY = m_previewIK.GetY(2) / 0.5f;
        m_overlayPanel->SetFields(fieldX, fieldY);
        Layout();
    }
    void RobotDrawPanel::Render(wxDC& dc)
    {
        Start();
        dc.Clear();

        RenderRobot(dc, m_shadowChain, SHADOW, ACCENT);
        RenderRobot(dc, m_previewChain, DEFAULT, (m_selected) ? SELECTED : ACCENT);

        m_overlayPanel->UpdateOverlay();
    }
    void RobotDrawPanel::RenderRobot(wxDC& dc, const Chain& chain, const wxColor& color, const wxColor& selected)
    {
        dc.SetPen(wxPen(ACCENT, 8));

        dc.DrawLine(chain.base, chain.mid);
        dc.DrawLine(chain.mid, chain.end);

        dc.SetPen(wxPen(color, 4));

        dc.DrawLine(chain.base, chain.mid);
        dc.DrawLine(chain.mid, chain.end);

        dc.SetBrush(wxBrush(color));
        dc.SetPen(wxPen(ACCENT, 2));

        dc.DrawCircle(chain.base, 10);
        dc.DrawCircle(chain.mid, 10);

        dc.SetPen(wxPen(selected, 2));
        dc.DrawCircle(chain.end, 10);
    }
    void RobotDrawPanel::OnLeftDown(wxMouseEvent& event)
    {
        double x2 = std::pow((static_cast<double>(event.m_x) - m_previewChain.end.x), 2);
        double y2 = std::pow((static_cast<double>(event.m_y) - m_previewChain.end.y), 2);
        double r2 = 10 * 10;
        if ((x2 + y2) < r2)
        {
            m_selected = true;
            PainNow();
        }
        event.Skip();
    }
    void RobotDrawPanel::OnLeftUp(wxMouseEvent& event)
    {
        if (m_selected)
        {
            m_selected = false;
            PainNow();
            event.SetId(0);
            wxPostEvent(GetParent(), event);
        }
        event.Skip();
    }
    void RobotDrawPanel::OnMotion(wxMouseEvent& event)
    {
        if (m_selected)
        {
            auto targetX = event.m_x;
            auto targetY = event.m_y;

            // mouse outside max position
            if (GetDistance(targetX, targetY, m_offset[0], m_offset[1]) > (m_sizes[0] + m_sizes[1]))
            {
                auto radians = static_cast<float>(atan2(targetY - m_offset[1], targetX - m_offset[0]));
                auto radius = (m_sizes[0] + m_sizes[1]);

                targetX = static_cast<int>(radius * cos(radians) + m_offset[0]);
                targetY = static_cast<int>(radius * sin(radians) + m_offset[1]);
            }

            auto targetPosX = static_cast<float>(targetX - m_offset[0]);
            auto targetPosY = static_cast<float>(targetY - m_offset[1]);

            int32_t sizes[] = {static_cast<int32_t>(m_sizes[0]), static_cast<int32_t>(m_sizes[1])};
            if (!m_previewIK.Solve(targetPosX, targetPosY, sizes))
            {
                // if there was no solve, don't update locations and angles.
                return;
            }
            double placeholderAngle = m_previewIK.GetAngle(0);
            placeholderAngle = TO_DEGREES(placeholderAngle);

            if (placeholderAngle < 0 && placeholderAngle >= -90)// right side border reached
            {
                int32_t centerX = m_offset[0] + sizes[0];
                int32_t centerY = m_offset[1];

                double angle1 = TO_DEGREES(m_previewIK.GetAngle(1));
                double angle0 = 0.5;

                if ((event.m_y < m_offset[1]) && (GetDistance(event.m_x, event.m_y, centerX, centerY) > m_sizes[1]))
                {
                    angle1 = GetAngle(event.m_x, event.m_y, centerX, centerY);
                    angle1 += -M_PI;
                    angle1 = TO_DEGREES(angle1);
                }

                UpdateAngles(std::array<double, 2>{angle0, angle1});
                m_currentPosition[0] = static_cast<float>(m_previewIK.GetAngle(0) - TO_RADIANS(90));
                m_currentPosition[1] = m_previewIK.GetAngle(2);
                m_staticOffset = 0;
            }
            else if (placeholderAngle < -90 && placeholderAngle >= -180)// left side border reached
            {
                int32_t centerX = m_offset[0] - sizes[0];
                int32_t centerY = m_offset[1];

                double angle1 = TO_DEGREES(m_previewIK.GetAngle(1));
                double angle0 = 90;

                if ((event.m_y < m_offset[1]) && (GetDistance(event.m_x, event.m_y, centerX, centerY) > m_sizes[1]))
                {
                    angle1 = GetAngle(event.m_x, event.m_y, centerX, centerY);
                    angle1 += -2 * M_PI;
                    angle1 = TO_DEGREES(angle1);
                }

                UpdateAngles(std::array<double, 2>{angle0, angle1});
                m_currentPosition[0] = static_cast<float>(m_previewIK.GetAngle(0));
                m_currentPosition[1] = m_previewIK.GetAngle(2);
                m_staticOffset = TO_DEGREES(m_currentPosition[0]);
            }
            else// no border reached, update normal
            {
                m_currentPosition[0] = static_cast<float>(m_previewIK.GetAngle(0) - TO_RADIANS(90));
                m_currentPosition[1] = m_previewIK.GetAngle(1);
                m_staticOffset = 0;
            }

            m_positionBuffer = m_currentPosition;

            UpdateJoints(m_previewChain, m_previewIK);
            UpdateOverlay();
            PainNow();

            event.SetId(0);// let parent now the event is coming from here.
            wxPostEvent(GetParent(), event);
        }
    }
    void RobotDrawPanel::InitializeFrame()
    {
        wxBoxSizer* bSizer14;
        bSizer14 = new wxBoxSizer(wxHORIZONTAL);
        m_overlayPanel = new Overlay(this);
        bSizer14->Add(m_overlayPanel, 0, wxALIGN_BOTTOM, 5);
        SetSizer(bSizer14);
        Layout();
        bSizer14->Fit(this);
    }

    /** ========================================================== **/
    /** OVERLAY--------------------------------------------OVERLAY **/
    /** ========================================================== **/

    void Overlay::SetX(float value)
    {
        m_textValueX->SetLabel(Utils::String::ToString(value, 2));
        Layout();
    }
    void Overlay::SetY(float value)
    {
        m_textValueY->SetLabel(Utils::String::ToString(value, 2));
        Layout();
    }
    void Overlay::SetFields(float fieldX, float fieldY)
    {
        m_textValueX->SetLabel(Utils::String::ToString(fieldX, 2));
        m_textValueY->SetLabel(Utils::String::ToString(fieldY, 2));
        Layout();
    }
    float Overlay::GetX() const
    {
        return std::stof(m_textValueX->GetLabel().ToStdString());
    }
    float Overlay::GetY() const
    {
        return std::stof(m_textValueY->GetLabel().ToStdString());
    }
    void Overlay::UpdateOverlay()
    {
        Refresh();
        Update();
    }
    void Overlay::InitializeFrame()
    {
        SetForegroundColour(wxColour{255, 255, 255});

        wxFlexGridSizer* fgSizer8;
        fgSizer8 = new wxFlexGridSizer(2, 3, 0, 0);
        fgSizer8->SetFlexibleDirection(wxBOTH);
        fgSizer8->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

        m_textX = new wxStaticText(this, wxID_ANY, wxT("X:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textX->Wrap(-1);
        fgSizer8->Add(m_textX, 0, wxALL, 5);

        m_textValueX = new wxStaticText(this, wxID_ANY, wxT("0.00"), wxDefaultPosition, wxDefaultSize, 0);
        m_textValueX->Wrap(-1);
        fgSizer8->Add(m_textValueX, 0, wxTOP | wxBOTTOM | wxLEFT, 5);

        m_iconX = new wxStaticText(this, wxID_ANY, wxT("mm"), wxDefaultPosition, wxDefaultSize, 0);
        m_iconX->Wrap(-1);
        fgSizer8->Add(m_iconX, 0, wxTOP | wxBOTTOM | wxRIGHT, 5);

        m_textY = new wxStaticText(this, wxID_ANY, wxT("Y:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textY->Wrap(-1);
        fgSizer8->Add(m_textY, 0, wxALL, 5);

        m_textValueY = new wxStaticText(this, wxID_ANY, wxT("0.00"), wxDefaultPosition, wxDefaultSize, 0);
        m_textValueY->Wrap(-1);
        fgSizer8->Add(m_textValueY, 0, wxTOP | wxBOTTOM | wxLEFT, 5);

        m_iconY = new wxStaticText(this, wxID_ANY, wxT("mm"), wxDefaultPosition, wxDefaultSize, 0);
        m_iconY->Wrap(-1);
        fgSizer8->Add(m_iconY, 0, wxTOP | wxBOTTOM | wxRIGHT, 5);

        SetSizer(fgSizer8);
        Layout();
        fgSizer8->Fit(this);
    }
}// namespace Frame