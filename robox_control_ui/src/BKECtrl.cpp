// ======================================
// Filename:    BKECtrl.h
// Project:     ROBOX Control UI
// Subsystem:   BKE control
// Author:      Luke van Luijn
// Data:        10-06-2022
// ======================================
#include <random>

#include "wx/sizer.h"

#include "BKECtrl.h"
#include "Logger.h"

#include "BKEDriver.h"
#include "Bitmap.h"
#include "Controller.h"
#include "MainApplication.h"
#include "RobotController.h"

#include "BKEStrategy.h"

namespace
{
    enum class Statistics_e : uint8_t
    {
        GAMES_PLAYED = 2,
        GAMES_WON = 3,
        GAMES_LOST = 4,
        GAMES_DRAW = 5,
    };

    const std::chrono::time_point<std::chrono::steady_clock> programStartTime = std::chrono::steady_clock::now();
    std::map<Statistics_e, uint32_t> statistics = {
            {Statistics_e::GAMES_PLAYED, 0},
            {Statistics_e::GAMES_WON, 0},
            {Statistics_e::GAMES_LOST, 0},
            {Statistics_e::GAMES_DRAW, 0},
    };
}// namespace

namespace Frame
{
    BKECtrl::BKECtrl(wxWindow* parent)
        : IFrame(parent), m_bitmapPieceO(Utils::Bitmap::CreateBitmap("piece_o.png", wxSize(80, 80))),
          m_bitmapPieceX(Utils::Bitmap::CreateBitmap("piece_x.png", wxSize(80, 80))),
          m_bitmapEmpty(Utils::Bitmap::SetTransparent(m_bitmapPieceX)), m_storage({5, 5}), m_gameActive(false),
          m_gameStart(std::chrono::steady_clock::now()), m_strategy(BKEStrategy_e::EASY), m_currentTurn(BKETurn_e::USER)
    {
        InitializeFrame();
        ConnectEvents();
        this->Connect(wxEVT_TIMER, wxTimerEventHandler(BKECtrl::OnUpdateEvent), nullptr, this);

        m_timer = new wxTimer;
        m_timer->SetOwner(this);
        m_timer->Start(1000);

        ToggleTurn(BKETurn_e::USER);
    }
    BKECtrl::~BKECtrl()
    {
        DisconnectEvents();
        this->Disconnect(wxEVT_TIMER, wxTimerEventHandler(BKECtrl::OnUpdateEvent), nullptr, this);
    }
    wxPanel* BKECtrl::GetPanel()
    {
        return this;
    }
    void BKECtrl::UpdateData(DataType_e responseType)
    {
        switch (responseType)
        {
            case DataType_e::HOMING_COMPLETE:
                INFO("Homing sequence complete");
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                SyncFields();
                break;
            case DataType_e::GET_STATIC:
                UpdateFields();
                break;
            case DataType_e::GET_RUNTIME:
            case DataType_e::GET_DYNAMIC:
            case DataType_e::SET_SPEED_ACCEL:
            case DataType_e::SET_VERBOSITY:
            case DataType_e::SET_FRAME_OFFSET:
            case DataType_e::SET_GRIP_OFFSET:
            case DataType_e::SET_IDLE_POSITIONS:
            default:
                break;
        }
        Refresh();
        Update();
    }
    void BKECtrl::ResetPanel()
    {
        // TODO
    }
    void BKECtrl::UpdateBKE(BKEType_e type)
    {
        switch (type)
        {
            case BKEType_e::BOARD_RESULT:
            case BKEType_e::BOARD_UPDATE:
            {
                int64_t piecesX = std::count_if(Driver::BKEDriver::m_board.begin(),
                                                Driver::BKEDriver::m_board.end(),
                                                [](uint16_t piece) {
                                                    return piece == (uint16_t) BKEPiece_e::X_PIECE;
                                                });
                m_storage[(size_t) StorageType_e::STORAGE_X] = static_cast<uint8_t>(5 - piecesX);
                int64_t piecesO = std::count_if(Driver::BKEDriver::m_board.begin(),
                                                Driver::BKEDriver::m_board.end(),
                                                [](uint16_t piece) {
                                                    return piece == (uint16_t) BKEPiece_e::O_PIECE;
                                                });
                m_storage[(size_t) StorageType_e::STORAGE_O] = static_cast<uint8_t>(5 - piecesO);

                if (type == BKEType_e::BOARD_UPDATE)
                {
                    BKEPiece_e newPiece;
                    if (Driver::BKEDriver::GetInstance().NewPiecePlaced(newPiece))
                    {
                        switch (newPiece)
                        {
                            case BKEPiece_e::X_PIECE:
                            {
                                ToggleTurn(BKETurn_e::ROBOX);

                                if (!m_gameActive)
                                {
                                    auto window = dynamic_cast<Base::Controller*>(Base::TheApp().GetTopWindow());
                                    if (window)
                                    {
                                        window->ActivateRobot();

                                        m_gameActive = true;
                                        m_gameStart = std::chrono::steady_clock::now();
                                        SyncFields();
                                        UpdateFields();
                                    }
                                }
                                uint8_t pieceIndex = 0;
                                if (PlayPiece(m_strategy, pieceIndex))
                                {
                                    BoardInteraction(pieceIndex);
                                }
                            }
                            break;
                            case BKEPiece_e::O_PIECE:
                                ToggleTurn(BKETurn_e::USER);
                                break;
                            case BKEPiece_e::NO_PIECE:
                                break;
                        }
                    }
                }

                for (std::size_t i = 0; i < Driver::BKEDriver::m_board.size(); ++i)
                {
                    switch (static_cast<BKEPiece_e>(Driver::BKEDriver::m_board[i]))
                    {
                        case BKEPiece_e::NO_PIECE:
                            m_boardButtons[i]->SetBitmap(m_bitmapEmpty);
                            break;
                        case BKEPiece_e::O_PIECE:
                            m_boardButtons[i]->SetBitmap(m_bitmapPieceO);
                            break;
                        case BKEPiece_e::X_PIECE:
                            m_boardButtons[i]->SetBitmap(m_bitmapPieceX);
                            break;
                    }
                    m_boardButtons[i]->Refresh();
                    m_boardButtons[i]->Update();
                }
            }
            break;
            case BKEType_e::GAME_RESULT:
            {
                const std::array<std::string, 3> results = {"Game won", "Game lost", "Game draw"};
                INFO("Game completed.", results[(size_t)Driver::BKEDriver::m_result]);
                Driver::RobotController::GetInstance().PauseRobot(1000);

                auto WaitAndClear = [&]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));// wait for board result
                    ClearBoard();

                    auto window = dynamic_cast<Base::Controller*>(Base::TheApp().GetTopWindow());
                    if (window)
                    {
                        window->DeactivateRobot();
                        m_gameActive = false;
                    }
                };
                std::thread clearBoardThread(WaitAndClear);
                clearBoardThread.detach();

                UpdateStatistics(Driver::BKEDriver::m_result);
            }
            break;
        }
    }
    void BKECtrl::UpdateFields()
    {
        m_minMaxValues[(size_t) Fields_e::FIELD_SPEED].first = 0;
        m_minMaxValues[(size_t) Fields_e::FIELD_SPEED].second = 100;
        m_minMaxValues[(size_t) Fields_e::FIELD_ACCEL].first = 0;
        m_minMaxValues[(size_t) Fields_e::FIELD_ACCEL].second = 100;

        m_sliderSpeed->SetMin(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_SPEED].first * 100));
        m_sliderSpeed->SetMax(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_SPEED].second * 100));

        m_sliderAccel->SetMin(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_ACCEL].first * 100));
        m_sliderAccel->SetMax(static_cast<int32_t>(m_minMaxValues[(size_t) Fields_e::FIELD_ACCEL].second * 100));

        Refresh();
        Update();

        INFO("BKECtrl fields updated");
    }
    void BKECtrl::SyncFields()
    {
        const float startSpeed = 100 * 100;
        const float startAccel = 100 * 100;

        m_fieldSpeed->SetValue(Utils::String::ToString(startSpeed / 100, 2));
        m_fieldAccel->SetValue(Utils::String::ToString(startAccel / 100, 2));

        m_sliderSpeed->SetValue(static_cast<int32_t>(startSpeed));
        m_sliderAccel->SetValue(static_cast<int32_t>(startAccel));

        Driver::RobotController::GetInstance().SetNewSpeed(startSpeed / 100);
        Driver::RobotController::GetInstance().SetNewAccel(startAccel / 100);

        m_sliderAccel->Refresh();
        m_sliderSpeed->Refresh();

        Refresh();
        Update();

        INFO("BKECtrl fields synchronized");
    }
    bool BKECtrl::OnTextEnter(wxTextCtrl* textCtrl, wxSlider* slider, float& value, Fields_e field)
    {
        float min = m_minMaxValues[(size_t) field].first;
        float max = m_minMaxValues[(size_t) field].second;
        std::string textValue = textCtrl->GetValue().ToStdString();

        if (!Utils::String::ValidateNumber(value, textValue, min, max))
        {
            textCtrl->SetForegroundColour(wxColour(255, 0, 0));
        }
        else
        {
            textCtrl->SetForegroundColour(wxColour(0, 0, 0));
            textCtrl->SetValue(Utils::String::ToString(value));
            slider->SetValue(static_cast<int32_t>(value * 100));
            INFO("New value set");
            return true;
        }
        ERROR("Could not translate input");
        return false;
    }
    void BKECtrl::OnSliderMove(wxTextCtrl* textCtrl, wxSlider* slider)
    {
        auto value = static_cast<float>(slider->GetValue()) / 100;
        textCtrl->SetValue(Utils::String::ToString(value, 2));
    }
    void BKECtrl::PlacePiece(uint8_t index, StorageType_e type, bool home)
    {
        BKELocation storageLocation{};
        if (!GetStorageLocation(type, StorageInteraction_e::RETRIEVE, storageLocation))
        {
            return;
        }

        MoveToLocation(storageLocation, false);
        MoveToLocation(storageLocation, true);

        TriggerGripper(255);

        MoveToLocation(storageLocation, false);
        MoveToLocation(m_locations[BOARD_START + index], false);
        MoveToLocation(m_locations[BOARD_START + index], true, true);

        TriggerGripper(0);

        MoveToLocation(m_locations[BOARD_START + index], false);

        if (home)
        {
            MoveToActiveHome();
        }
    }
    void BKECtrl::RemovePiece(uint8_t index, StorageType_e type, bool home)
    {
        BKELocation storageLocation{};
        if (!GetStorageLocation(type, StorageInteraction_e::STORE, storageLocation))
        {
            return;
        }
        MoveToLocation(m_locations[BOARD_START + index], false);
        MoveToLocation(m_locations[BOARD_START + index], true);

        TriggerGripper(255);

        MoveToLocation(m_locations[BOARD_START + index], false);
        MoveToLocation(storageLocation, false);
        MoveToLocation(storageLocation, true, true);

        TriggerGripper(0);

        MoveToLocation(storageLocation, false);

        if (home)
        {
            MoveToActiveHome();
        }
    }
    bool BKECtrl::GetStorageLocation(StorageType_e type, StorageInteraction_e interaction, BKELocation& location)
    {
        if (interaction == StorageInteraction_e::RETRIEVE)
        {
            int64_t index = m_storage[(size_t) type];

            if (index == 0)
            {
                ERROR("Storage is empty for this type.");
                return false;
            }

            int64_t storageIndex =
                    (type == StorageType_e::STORAGE_O) ? STORAGE_O_START + (index - 1) : STORAGE_X_START + (index - 1);
            location = m_locations[static_cast<uint64_t>(storageIndex)];
            --m_storage[(size_t) type];
            return true;
        }
        else// StorageInteraction_e::STORE
        {
            int64_t index = m_storage[(size_t) type];

            if (index >= 5)
            {
                ERROR("Storage is full for this type.");
                return false;
            }
            int64_t storageIndex =
                    (type == StorageType_e::STORAGE_O) ? STORAGE_O_START + (index) : STORAGE_X_START + (index);
            location = m_locations[static_cast<uint64_t>(storageIndex)];
            ++m_storage[(size_t) type];
            return true;
        }
    }
    void BKECtrl::BoardInteraction(uint8_t index, bool home)
    {
        if (Driver::BKEDriver::m_board[index] == (uint8_t) BKEPiece_e::NO_PIECE)
        {
            PlacePiece(index, StorageType_e::STORAGE_O, home);
            INFO("Placing O-Piece", (uint16_t)index);
        }
        else
        {
            if (Driver::BKEDriver::m_board[index] == (uint8_t) BKEPiece_e::O_PIECE)
            {
                RemovePiece(index, StorageType_e::STORAGE_O, home);
                INFO("Removing O-Piece", (uint16_t)index);
            }
            else// BKEPiece_e::X_PIECE
            {
                RemovePiece(index, StorageType_e::STORAGE_X, home);
                INFO("Removing X-Piece", (uint16_t)index);
            }
        }
    }
    void BKECtrl::ClearBoard()
    {
        if (Driver::BKEDriver::m_board[4] != (uint8_t) BKEPiece_e::NO_PIECE)
        {
            BoardInteraction(static_cast<uint8_t>(4), false);
        }

        for (size_t i = 0; i < Driver::BKEDriver::m_board.size(); ++i)
        {
            if (i == 4)
            {
                continue;
            }
            if (Driver::BKEDriver::m_board[i] != (uint8_t) BKEPiece_e::NO_PIECE)
            {
                BoardInteraction(static_cast<uint8_t>(i), false);
            }
        }
    }
    void BKECtrl::ToggleTurn(BKETurn_e turn)
    {
        auto toggleTurn = [](wxStaticText* turn, wxStaticText* notTurn) {
            turn->SetBackgroundColour(wxColor(0, 255, 0));
            notTurn->SetBackgroundColour(wxColor(80, 80, 80));

            turn->SetForegroundColour(wxColor(0, 0, 0));
            notTurn->SetForegroundColour(wxColor(255, 255, 255));
        };

        switch (turn)
        {
            case BKETurn_e::ROBOX:
            {
                m_textTurnROBOX->SetLabel("ROBOX <-");
                m_textTurnUser->SetLabel("USER");
                toggleTurn(m_textTurnROBOX, m_textTurnUser);
            }
            break;
            case BKETurn_e::USER:
            {
                m_textTurnROBOX->SetLabel("ROBOX");
                m_textTurnUser->SetLabel("-> USER");
                toggleTurn(m_textTurnUser, m_textTurnROBOX);
            }
            break;
        }

        Layout();
        Refresh();
    }
    void BKECtrl::UpdateStatistics(BKEResult_e result)
    {
        ++statistics[Statistics_e::GAMES_PLAYED];
        switch (result)
        {
            case BKEResult_e::GAME_WON:
                ++statistics[Statistics_e::GAMES_WON];
                break;
            case BKEResult_e::GAME_LOST:
                ++statistics[Statistics_e::GAMES_LOST];
                break;
            case BKEResult_e::GAME_DRAW:
                ++statistics[Statistics_e::GAMES_DRAW];
                break;
        }

        float wonPercentage =
                static_cast<float>(statistics[Statistics_e::GAMES_WON] / statistics[Statistics_e::GAMES_PLAYED]) * 100;
        float lostPercentage =
                static_cast<float>(statistics[Statistics_e::GAMES_LOST] / statistics[Statistics_e::GAMES_PLAYED]) * 100;
        float drawPercentage =
                static_cast<float>(statistics[Statistics_e::GAMES_DRAW] / statistics[Statistics_e::GAMES_PLAYED]) * 100;

        m_textGamesPlayedValue->SetLabel(std::to_string(statistics[Statistics_e::GAMES_PLAYED]));
        m_textWonValue->SetLabel(std::to_string(statistics[Statistics_e::GAMES_WON]));
        m_textLostValue->SetLabel(std::to_string(statistics[Statistics_e::GAMES_LOST]));
        m_textDrawValue->SetLabel(std::to_string(statistics[Statistics_e::GAMES_DRAW]));

        m_textWonPercentage->SetLabel(Utils::String::ToString(wonPercentage, 2));
        m_textLostPercentage->SetLabel(Utils::String::ToString(lostPercentage, 2));
        m_textDrawPercentage->SetLabel(Utils::String::ToString(drawPercentage, 2));
    }
    void BKECtrl::MoveToActiveHome()
    {
        float A0 = Driver::RobotDriver::m_dataManager.m_segment00.m_idlePosition / 2;
        float A1 = Driver::RobotDriver::m_dataManager.m_segment01.m_idlePosition;
        float A2 = Driver::RobotDriver::m_dataManager.m_segment02.m_idlePosition - 360;
        float A3 = Driver::RobotDriver::m_dataManager.m_segment03.m_idlePosition;
        Move(A0, A1, A2, A3);
    }
    void BKECtrl::MoveToLocation(const BKELocation& location, bool isTrigger, bool isDrop)
    {
        float A0 = (isTrigger) ? location.h : location.h + 50.f;
        A0 += (isDrop) ? 5 : 0;
        float A1 = location.a1;
        float A2 = location.a2;
        float A3 = GetGripAngle(A1, A2);
        Move(A0, A1, A2, A3);
    }
    float BKECtrl::GetGripAngle(float A1, float A2)
    {
        const float L1 = Driver::RobotDriver::m_dataManager.m_lengthSegment01;
        const float L2 = Driver::RobotDriver::m_dataManager.m_lengthSegment02;
        const auto A1F = static_cast<float>(A1 * M_PI / 180);
        const auto A2F = static_cast<float>(A2 * M_PI / 180);
        const float xPos = round(L1 * cos(A1F) + L2 * cos(A1F + A2F));
        const float yPos = round(L1 * sin(A1F) + L2 * sin(A1F + A2F));

        // https://howtomechatronics.com/projects/scara-robot-how-to-build-your-own-arduino-based-robot/

        float A3 = 90 + A1 + A2;
        A3 = (-1) * A3;

        if ((xPos < 0) && (yPos < 0))
        {
            A3 = 270 - A1 - A2;
        }

        A3 += 90;
        return A3;
    }
    void BKECtrl::TriggerGripper(uint8_t value)
    {
        Driver::RobotController::GetInstance().SetGripperValue01(value);
        Driver::RobotController::GetInstance().SetGripperValue02(value);
        Driver::RobotController::GetInstance().RunRobot();
    }
    void BKECtrl::Move(float A0, float A1, float A2, float A3)
    {
        Driver::RobotController::GetInstance().SetNewPositionA0(A0);
        Driver::RobotController::GetInstance().SetNewPositionA1(A1);
        Driver::RobotController::GetInstance().SetNewPositionA2(A2);
        Driver::RobotController::GetInstance().SetNewPositionA3(A3);
        Driver::RobotController::GetInstance().RunRobot();
    }
    bool BKECtrl::PlayPiece(BKEStrategy_e strategy, uint8_t& index)
    {
        switch (strategy)
        {
            case BKEStrategy_e::FAIR:
            {
                int16_t result = Utils::BKEStrategy::CheckBoardLines(BKEPiece_e::O_PIECE,BKEPiece_e::X_PIECE);
                if (result != -1)
                {
                    index = static_cast<uint8_t>(result);
                    return true;
                }
                result = Utils::BKEStrategy::CheckBoardLines(BKEPiece_e::X_PIECE,BKEPiece_e::O_PIECE);
                if (result != -1)
                {
                    index = static_cast<uint8_t>(result);
                    return true;
                }
                result = Utils::BKEStrategy::CheckMove(BKEPiece_e::X_PIECE);
                if (result != -1)
                {
                    index = static_cast<uint8_t>(result);
                    return true;
                }
                // no valid solution found, pick any
                WARNING("No viable move found, picked random");
                return PlayPiece(BKEStrategy_e::EASY, index);
            }
            case BKEStrategy_e::EASY:
            {
                if (Utils::BKEStrategy::IsBoardFilled())
                {
                    INFO("No move possible, board is full.");
                    return false;
                }

                std::random_device dev;
                std::mt19937 rng(dev());
                std::uniform_int_distribution<std::mt19937::result_type> dist(0, 8);

                index = static_cast<uint8_t>(dist(rng));
                while (Driver::BKEDriver::m_board[index] != (uint8_t) BKEPiece_e::NO_PIECE)
                {
                    index = static_cast<uint8_t>(dist(rng));
                }
                index += BOARD_START;
                return true;
            }
            case BKEStrategy_e::MANUAL:
                // do nothing.
                break;
        }
        return false;
    }
    void BKECtrl::OnUpdateEvent(wxTimerEvent& event)
    {
        typedef std::chrono::time_point<std::chrono::steady_clock> TP;

        auto convert = [](int64_t time) -> std::string {
            std::string timeStr = std::to_string(time);
            if (time < 10)
            {
                timeStr = "0" + timeStr;
            }
            return timeStr;
        };

        auto getTime = [&convert](const TP& start) -> std::string {
            auto now = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

            auto hours = static_cast<uint16_t>(std::floor(duration / 3600));
            duration = duration % 3600;
            auto minutes = static_cast<uint16_t>(std::floor(duration / 60));
            duration = duration % 60;
            auto seconds = static_cast<uint16_t>(duration);
            std::string time = convert(hours) + ":" + convert(minutes) + ":" + convert(seconds);
            return time;
        };

        m_textRunTimeValue->SetLabel(getTime(programStartTime));
        m_textGameTimeValue->SetLabel((m_gameActive) ? getTime(m_gameStart) : "n/a [idle]");

        Layout();

        event.Skip();
    }
    void BKECtrl::OnKillFocusSpeed(wxFocusEvent& event)
    {
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
        OnTextEnterSpeed(commandEvent);
        event.Skip();
    }
    void BKECtrl::OnTextEnterSpeed(wxCommandEvent& event)
    {
        float value = 0.f;
        if (OnTextEnter(m_fieldSpeed, m_sliderSpeed, value, Fields_e::FIELD_SPEED))
        {
            Driver::RobotController::GetInstance().SetNewSpeed(value);
        }
        event.Skip();
    }
    void BKECtrl::OnTextMaxLenSpeed(wxCommandEvent& event)
    {
        WARNING("Max amount of characters reached.", "6");
        event.Skip();
    }
    void BKECtrl::OnSliderReleaseSpeed(wxScrollEvent& event)
    {
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
        OnTextEnterSpeed(commandEvent);
        event.Skip();
    }
    void BKECtrl::OnSliderSpeed(wxCommandEvent& event)
    {
        OnSliderMove(m_fieldSpeed, m_sliderSpeed);
        event.Skip();
    }
    void BKECtrl::OnKillFocusAccel(wxFocusEvent& event)
    {
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
        OnTextEnterAccel(commandEvent);
        event.Skip();
    }
    void BKECtrl::OnTextEnterAccel(wxCommandEvent& event)
    {
        float value = 0.f;
        if (OnTextEnter(m_fieldAccel, m_sliderAccel, value, Fields_e::FIELD_ACCEL))
        {
            Driver::RobotController::GetInstance().SetNewAccel(value);
        }
        event.Skip();
    }
    void BKECtrl::OnTextMaxLenAccel(wxCommandEvent& event)
    {
        WARNING("Max amount of characters reached.", "6");
        event.Skip();
    }
    void BKECtrl::OnSliderReleaseAccel(wxScrollEvent& event)
    {
        wxCommandEvent commandEvent(wxEVT_COMMAND_TEXT_ENTER);
        OnTextEnterAccel(commandEvent);
        event.Skip();
    }
    void BKECtrl::OnSliderAccel(wxCommandEvent& event)
    {
        OnSliderMove(m_fieldAccel, m_sliderAccel);
        event.Skip();
    }
    void BKECtrl::OnClickResetBoard(wxCommandEvent& event)
    {
        INFO("Clearing the board");
        ClearBoard();
        MoveToActiveHome();
        event.Skip();
    }
    void BKECtrl::OnClickGoToHome(wxCommandEvent& event)
    {
        MoveToActiveHome();
        event.Skip();
    }
    void BKECtrl::OnClickBoard00(wxCommandEvent& event)
    {
        BoardInteraction(0);
        event.Skip();
    }
    void BKECtrl::OnClickBoard01(wxCommandEvent& event)
    {
        BoardInteraction(1);
        event.Skip();
    }
    void BKECtrl::OnClickBoard02(wxCommandEvent& event)
    {
        BoardInteraction(2);
        event.Skip();
    }
    void BKECtrl::OnClickBoard03(wxCommandEvent& event)
    {
        BoardInteraction(3);
        event.Skip();
    }
    void BKECtrl::OnClickBoard04(wxCommandEvent& event)
    {
        BoardInteraction(4);
        event.Skip();
    }
    void BKECtrl::OnClickBoard05(wxCommandEvent& event)
    {
        BoardInteraction(5);
        event.Skip();
    }
    void BKECtrl::OnClickBoard06(wxCommandEvent& event)
    {
        BoardInteraction(6);
        event.Skip();
    }
    void BKECtrl::OnClickBoard07(wxCommandEvent& event)
    {
        BoardInteraction(7);
        event.Skip();
    }
    void BKECtrl::OnClickBoard08(wxCommandEvent& event)
    {
        BoardInteraction(8);
        event.Skip();
    }
    void BKECtrl::OnToggleManual(wxCommandEvent& event)
    {
        m_strategy = BKEStrategy_e::MANUAL;
        m_strategyButtonEasy->SetValue(false);
        m_strategyButtonFair->SetValue(false);
        event.Skip();
    }
    void BKECtrl::OnToggleFair(wxCommandEvent& event)
    {
        m_strategy = BKEStrategy_e::FAIR;
        m_strategyButtonEasy->SetValue(false);
        m_strategyButtonManual->SetValue(false);
        event.Skip();
    }
    void BKECtrl::OnToggleEasy(wxCommandEvent& event)
    {
        m_strategy = BKEStrategy_e::EASY;
        m_strategyButtonManual->SetValue(false);
        m_strategyButtonFair->SetValue(false);
        event.Skip();
    }
    void BKECtrl::InitializeFrame()
    {
        wxBoxSizer* mainSizer;
        mainSizer = new wxBoxSizer(wxVERTICAL);

        m_speedPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxFlexGridSizer* fgSizer7;
        fgSizer7 = new wxFlexGridSizer(2, 4, 0, 0);
        fgSizer7->AddGrowableCol(3);
        fgSizer7->SetFlexibleDirection(wxBOTH);
        fgSizer7->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

        m_textSpeed = new wxStaticText(m_speedPanel, wxID_ANY, wxT("Speed:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textSpeed->Wrap(-1);
        fgSizer7->Add(m_textSpeed, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

        m_fieldSpeed = new wxTextCtrl(m_speedPanel,
                                      wxID_ANY,
                                      wxEmptyString,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxTE_PROCESS_ENTER);
        fgSizer7->Add(m_fieldSpeed, 0, wxTOP | wxBOTTOM | wxLEFT, 5);

        m_iconSpeed = new wxStaticText(m_speedPanel, wxID_ANY, wxT("%"), wxDefaultPosition, wxDefaultSize, 0);
        m_iconSpeed->Wrap(-1);
        fgSizer7->Add(m_iconSpeed, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 5);

        m_sliderSpeed = new wxSlider(m_speedPanel,
                                     wxID_ANY,
                                     10000,
                                     0,
                                     10000,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxSL_HORIZONTAL);
        fgSizer7->Add(m_sliderSpeed, 0, wxEXPAND | wxTOP | wxBOTTOM | wxRIGHT, 5);

        m_textAccel = new wxStaticText(m_speedPanel, wxID_ANY, wxT("Accel:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textAccel->Wrap(-1);
        fgSizer7->Add(m_textAccel, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

        m_fieldAccel = new wxTextCtrl(m_speedPanel,
                                      wxID_ANY,
                                      wxEmptyString,
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxTE_PROCESS_ENTER);
        fgSizer7->Add(m_fieldAccel, 0, wxTOP | wxBOTTOM | wxLEFT, 5);

        m_iconAccel = new wxStaticText(m_speedPanel, wxID_ANY, wxT("%"), wxDefaultPosition, wxDefaultSize, 0);
        m_iconAccel->Wrap(-1);
        fgSizer7->Add(m_iconAccel, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 5);

        m_sliderAccel = new wxSlider(m_speedPanel,
                                     wxID_ANY,
                                     10000,
                                     0,
                                     10000,
                                     wxDefaultPosition,
                                     wxDefaultSize,
                                     wxSL_HORIZONTAL);
        fgSizer7->Add(m_sliderAccel, 0, wxEXPAND | wxTOP | wxBOTTOM | wxRIGHT, 5);


        m_speedPanel->SetSizer(fgSizer7);
        m_speedPanel->Layout();
        fgSizer7->Fit(m_speedPanel);
        mainSizer->Add(m_speedPanel, 0, wxEXPAND, 5);

        m_staticLine21 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        mainSizer->Add(m_staticLine21, 0, wxEXPAND | wxALL, 5);

        m_turnPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxBoxSizer* bSizer29;
        bSizer29 = new wxBoxSizer(wxHORIZONTAL);

        m_textTurn = new wxStaticText(m_turnPanel, wxID_ANY, wxT("Turn:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textTurn->Wrap(-1);
        bSizer29->Add(m_textTurn, 0, wxALL, 5);

        m_staticLine25 = new wxStaticLine(m_turnPanel,
                                          wxID_ANY,
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxLI_HORIZONTAL | wxLI_VERTICAL);
        bSizer29->Add(m_staticLine25, 0, wxEXPAND | wxALL, 5);

        m_textTurnROBOX = new wxStaticText(m_turnPanel,
                                           wxID_ANY,
                                           wxT("ROBOX"),
                                           wxDefaultPosition,
                                           wxDefaultSize,
                                           wxALIGN_CENTER_HORIZONTAL);
        m_textTurnROBOX->Wrap(-1);
        bSizer29->Add(m_textTurnROBOX, 1, wxALL, 5);

        m_staticLine24 = new wxStaticLine(m_turnPanel,
                                          wxID_ANY,
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxLI_HORIZONTAL | wxLI_VERTICAL);
        bSizer29->Add(m_staticLine24, 0, wxEXPAND | wxALL, 5);

        m_textTurnUser = new wxStaticText(m_turnPanel,
                                          wxID_ANY,
                                          wxT("User"),
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxALIGN_CENTER_HORIZONTAL);
        m_textTurnUser->Wrap(-1);
        bSizer29->Add(m_textTurnUser, 1, wxALL, 5);


        m_turnPanel->SetSizer(bSizer29);
        m_turnPanel->Layout();
        bSizer29->Fit(m_turnPanel);
        mainSizer->Add(m_turnPanel, 0, wxEXPAND, 5);

        m_staticLine22 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        mainSizer->Add(m_staticLine22, 0, wxEXPAND | wxALL, 5);

        m_boardPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxBoxSizer* bSizer30;
        bSizer30 = new wxBoxSizer(wxHORIZONTAL);

        m_panel41 = new wxPanel(m_boardPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxBoxSizer* bSizer32;
        bSizer32 = new wxBoxSizer(wxVERTICAL);

        m_buttonResetBoard = new wxButton(m_panel41, wxID_ANY, wxT("Reset board"), wxDefaultPosition, wxDefaultSize, 0);
        bSizer32->Add(m_buttonResetBoard, 1, wxALL | wxEXPAND, 5);

        m_buttonHome = new wxButton(m_panel41, wxID_ANY, wxT("Go to home"), wxDefaultPosition, wxDefaultSize, 0);
        bSizer32->Add(m_buttonHome, 1, wxALL | wxEXPAND, 5);

        m_strategyButtonManual =
                new wxToggleButton(m_panel41, wxID_ANY, wxT("Manual"), wxDefaultPosition, wxDefaultSize, 0);
        bSizer32->Add(m_strategyButtonManual, 0, wxALL | wxEXPAND, 5);

        m_strategyButtonFair =
                new wxToggleButton(m_panel41, wxID_ANY, wxT("Fair"), wxDefaultPosition, wxDefaultSize, 0);
        bSizer32->Add(m_strategyButtonFair, 0, wxALL | wxEXPAND, 5);

        m_strategyButtonEasy =
                new wxToggleButton(m_panel41, wxID_ANY, wxT("Easy"), wxDefaultPosition, wxDefaultSize, 0);
        m_strategyButtonEasy->SetValue(true);
        bSizer32->Add(m_strategyButtonEasy, 0, wxALL | wxEXPAND, 5);

        m_panel41->SetSizer(bSizer32);
        m_panel41->Layout();
        bSizer32->Fit(m_panel41);
        bSizer30->Add(m_panel41, 0, wxEXPAND | wxALL, 5);

        m_panel39 = new wxPanel(m_boardPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxGridSizer* gSizer5;
        gSizer5 = new wxGridSizer(3, 3, 0, 0);

        m_boardButtons[0] = new wxBitmapButton(m_panel39,
                                               wxID_ANY,
                                               wxNullBitmap,
                                               wxDefaultPosition,
                                               wxDefaultSize,
                                               wxBU_AUTODRAW | 0);
        gSizer5->Add(m_boardButtons[0], 1, wxALL | wxEXPAND, 5);

        m_boardButtons[1] = new wxBitmapButton(m_panel39,
                                               wxID_ANY,
                                               wxNullBitmap,
                                               wxDefaultPosition,
                                               wxDefaultSize,
                                               wxBU_AUTODRAW | 0);
        gSizer5->Add(m_boardButtons[1], 1, wxALL | wxEXPAND, 5);

        m_boardButtons[2] = new wxBitmapButton(m_panel39,
                                               wxID_ANY,
                                               wxNullBitmap,
                                               wxDefaultPosition,
                                               wxDefaultSize,
                                               wxBU_AUTODRAW | 0);
        gSizer5->Add(m_boardButtons[2], 1, wxALL | wxEXPAND, 5);

        m_boardButtons[3] = new wxBitmapButton(m_panel39,
                                               wxID_ANY,
                                               wxNullBitmap,
                                               wxDefaultPosition,
                                               wxDefaultSize,
                                               wxBU_AUTODRAW | 0);
        gSizer5->Add(m_boardButtons[3], 1, wxALL | wxEXPAND, 5);

        m_boardButtons[4] = new wxBitmapButton(m_panel39,
                                               wxID_ANY,
                                               wxNullBitmap,
                                               wxDefaultPosition,
                                               wxDefaultSize,
                                               wxBU_AUTODRAW | 0);
        gSizer5->Add(m_boardButtons[4], 1, wxALL | wxEXPAND, 5);

        m_boardButtons[5] = new wxBitmapButton(m_panel39,
                                               wxID_ANY,
                                               wxNullBitmap,
                                               wxDefaultPosition,
                                               wxDefaultSize,
                                               wxBU_AUTODRAW | 0);
        gSizer5->Add(m_boardButtons[5], 1, wxALL | wxEXPAND, 5);

        m_boardButtons[6] = new wxBitmapButton(m_panel39,
                                               wxID_ANY,
                                               wxNullBitmap,
                                               wxDefaultPosition,
                                               wxDefaultSize,
                                               wxBU_AUTODRAW | 0);
        gSizer5->Add(m_boardButtons[6], 1, wxALL | wxEXPAND, 5);

        m_boardButtons[7] = new wxBitmapButton(m_panel39,
                                               wxID_ANY,
                                               wxNullBitmap,
                                               wxDefaultPosition,
                                               wxDefaultSize,
                                               wxBU_AUTODRAW | 0);
        gSizer5->Add(m_boardButtons[7], 1, wxALL | wxEXPAND, 5);

        m_boardButtons[8] = new wxBitmapButton(m_panel39,
                                               wxID_ANY,
                                               wxNullBitmap,
                                               wxDefaultPosition,
                                               wxDefaultSize,
                                               wxBU_AUTODRAW | 0);
        gSizer5->Add(m_boardButtons[8], 1, wxALL | wxEXPAND, 5);


        m_panel39->SetSizer(gSizer5);
        m_panel39->Layout();
        gSizer5->Fit(m_panel39);
        bSizer30->Add(m_panel39, 1, wxEXPAND | wxALL, 5);


        m_boardPanel->SetSizer(bSizer30);
        m_boardPanel->Layout();
        bSizer30->Fit(m_boardPanel);
        mainSizer->Add(m_boardPanel, 1, wxEXPAND, 5);

        m_staticLine23 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
        mainSizer->Add(m_staticLine23, 0, wxEXPAND | wxALL, 5);

        m_statsPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxBoxSizer* bSizer33;
        bSizer33 = new wxBoxSizer(wxHORIZONTAL);

        m_panel42 = new wxPanel(m_statsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxFlexGridSizer* fgSizer8;
        fgSizer8 = new wxFlexGridSizer(3, 2, 0, 0);
        fgSizer8->AddGrowableCol(0);
        fgSizer8->AddGrowableCol(1);
        fgSizer8->SetFlexibleDirection(wxBOTH);
        fgSizer8->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

        m_textRunTime = new wxStaticText(m_panel42, wxID_ANY, wxT("Run time:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textRunTime->Wrap(-1);
        fgSizer8->Add(m_textRunTime, 1, wxALL, 5);

        m_textRunTimeValue =
                new wxStaticText(m_panel42, wxID_ANY, wxT("00:00:00"), wxDefaultPosition, wxDefaultSize, 0);
        m_textRunTimeValue->Wrap(-1);
        fgSizer8->Add(m_textRunTimeValue, 1, wxALL, 5);

        m_textGameTime = new wxStaticText(m_panel42, wxID_ANY, wxT("Play time:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textGameTime->Wrap(-1);
        fgSizer8->Add(m_textGameTime, 1, wxALL, 5);

        m_textGameTimeValue =
                new wxStaticText(m_panel42, wxID_ANY, wxT("00:00:00"), wxDefaultPosition, wxDefaultSize, 0);
        m_textGameTimeValue->Wrap(-1);
        fgSizer8->Add(m_textGameTimeValue, 1, wxALL, 5);

        m_textGamesPlayed =
                new wxStaticText(m_panel42, wxID_ANY, wxT("Games played:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textGamesPlayed->Wrap(-1);
        fgSizer8->Add(m_textGamesPlayed, 1, wxALL, 5);

        m_textGamesPlayedValue = new wxStaticText(m_panel42, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0);
        m_textGamesPlayedValue->Wrap(-1);
        fgSizer8->Add(m_textGamesPlayedValue, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);


        m_panel42->SetSizer(fgSizer8);
        m_panel42->Layout();
        fgSizer8->Fit(m_panel42);
        bSizer33->Add(m_panel42, 1, wxEXPAND | wxALL, 5);

        m_staticLine28 = new wxStaticLine(m_statsPanel,
                                          wxID_ANY,
                                          wxDefaultPosition,
                                          wxDefaultSize,
                                          wxLI_HORIZONTAL | wxLI_VERTICAL);
        bSizer33->Add(m_staticLine28, 0, wxEXPAND | wxALL, 5);

        m_panel43 = new wxPanel(m_statsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        wxFlexGridSizer* fgSizer9;
        fgSizer9 = new wxFlexGridSizer(3, 5, 0, 0);
        fgSizer9->SetFlexibleDirection(wxBOTH);
        fgSizer9->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

        m_textWon = new wxStaticText(m_panel43, wxID_ANY, wxT("Won:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textWon->Wrap(-1);
        fgSizer9->Add(m_textWon, 0, wxALL, 5);

        m_textWonValue = new wxStaticText(m_panel43, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0);
        m_textWonValue->Wrap(-1);
        fgSizer9->Add(m_textWonValue, 0, wxALL, 5);

        m_iconWon0 = new wxStaticText(m_panel43, wxID_ANY, wxT("("), wxDefaultPosition, wxDefaultSize, 0);
        m_iconWon0->Wrap(-1);
        fgSizer9->Add(m_iconWon0, 0, wxTOP | wxBOTTOM | wxLEFT, 5);

        m_textWonPercentage = new wxStaticText(m_panel43, wxID_ANY, wxT("n/a"), wxDefaultPosition, wxDefaultSize, 0);
        m_textWonPercentage->Wrap(-1);
        fgSizer9->Add(m_textWonPercentage, 0, wxTOP | wxBOTTOM, 5);

        m_IconWon1 = new wxStaticText(m_panel43, wxID_ANY, wxT("%)"), wxDefaultPosition, wxDefaultSize, 0);
        m_IconWon1->Wrap(-1);
        fgSizer9->Add(m_IconWon1, 0, wxTOP | wxBOTTOM | wxRIGHT, 5);

        m_textLost = new wxStaticText(m_panel43, wxID_ANY, wxT("Lost:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textLost->Wrap(-1);
        fgSizer9->Add(m_textLost, 0, wxALL, 5);

        m_textLostValue = new wxStaticText(m_panel43, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0);
        m_textLostValue->Wrap(-1);
        fgSizer9->Add(m_textLostValue, 0, wxALL, 5);

        m_iconLost0 = new wxStaticText(m_panel43, wxID_ANY, wxT("("), wxDefaultPosition, wxDefaultSize, 0);
        m_iconLost0->Wrap(-1);
        fgSizer9->Add(m_iconLost0, 0, wxTOP | wxBOTTOM | wxLEFT, 5);

        m_textLostPercentage = new wxStaticText(m_panel43, wxID_ANY, wxT("n/a"), wxDefaultPosition, wxDefaultSize, 0);
        m_textLostPercentage->Wrap(-1);
        fgSizer9->Add(m_textLostPercentage, 0, wxTOP | wxBOTTOM, 5);

        m_iconLost1 = new wxStaticText(m_panel43, wxID_ANY, wxT("%)"), wxDefaultPosition, wxDefaultSize, 0);
        m_iconLost1->Wrap(-1);
        fgSizer9->Add(m_iconLost1, 0, wxTOP | wxBOTTOM | wxRIGHT, 5);

        m_textDraw = new wxStaticText(m_panel43, wxID_ANY, wxT("Draw:"), wxDefaultPosition, wxDefaultSize, 0);
        m_textDraw->Wrap(-1);
        fgSizer9->Add(m_textDraw, 0, wxALL, 5);

        m_textDrawValue = new wxStaticText(m_panel43, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0);
        m_textDrawValue->Wrap(-1);
        fgSizer9->Add(m_textDrawValue, 0, wxALL, 5);

        m_iconDraw0 = new wxStaticText(m_panel43, wxID_ANY, wxT("("), wxDefaultPosition, wxDefaultSize, 0);
        m_iconDraw0->Wrap(-1);
        fgSizer9->Add(m_iconDraw0, 0, wxTOP | wxBOTTOM | wxLEFT, 5);

        m_textDrawPercentage = new wxStaticText(m_panel43, wxID_ANY, wxT("n/a"), wxDefaultPosition, wxDefaultSize, 0);
        m_textDrawPercentage->Wrap(-1);
        fgSizer9->Add(m_textDrawPercentage, 0, wxTOP | wxBOTTOM, 5);

        m_iconDraw1 = new wxStaticText(m_panel43, wxID_ANY, wxT("%)"), wxDefaultPosition, wxDefaultSize, 0);
        m_iconDraw1->Wrap(-1);
        fgSizer9->Add(m_iconDraw1, 0, wxTOP | wxBOTTOM | wxRIGHT, 5);


        m_panel43->SetSizer(fgSizer9);
        m_panel43->Layout();
        fgSizer9->Fit(m_panel43);
        bSizer33->Add(m_panel43, 1, wxEXPAND | wxALL, 5);


        m_statsPanel->SetSizer(bSizer33);
        m_statsPanel->Layout();
        bSizer33->Fit(m_statsPanel);
        mainSizer->Add(m_statsPanel, 0, wxEXPAND, 5);

        this->SetSizer(mainSizer);
        this->Layout();
        mainSizer->Fit(this);

        //        Disable();
    }
    void BKECtrl::ConnectEvents()
    {
        m_fieldSpeed->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(BKECtrl::OnKillFocusSpeed), nullptr, this);
        m_fieldSpeed->Connect(wxEVT_COMMAND_TEXT_ENTER,
                              wxCommandEventHandler(BKECtrl::OnTextEnterSpeed),
                              nullptr,
                              this);
        m_fieldSpeed->Connect(wxEVT_COMMAND_TEXT_MAXLEN,
                              wxCommandEventHandler(BKECtrl::OnTextMaxLenSpeed),
                              nullptr,
                              this);
        m_sliderSpeed->Connect(wxEVT_SCROLL_THUMBRELEASE,
                               wxScrollEventHandler(BKECtrl::OnSliderReleaseSpeed),
                               nullptr,
                               this);
        m_sliderSpeed->Connect(wxEVT_SLIDER, wxCommandEventHandler(BKECtrl::OnSliderSpeed), nullptr, this);
        m_fieldAccel->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(BKECtrl::OnKillFocusAccel), nullptr, this);
        m_fieldAccel->Connect(wxEVT_COMMAND_TEXT_ENTER,
                              wxCommandEventHandler(BKECtrl::OnTextEnterAccel),
                              nullptr,
                              this);
        m_fieldAccel->Connect(wxEVT_COMMAND_TEXT_MAXLEN,
                              wxCommandEventHandler(BKECtrl::OnTextMaxLenAccel),
                              nullptr,
                              this);
        m_sliderAccel->Connect(wxEVT_SCROLL_THUMBRELEASE,
                               wxScrollEventHandler(BKECtrl::OnSliderReleaseAccel),
                               nullptr,
                               this);
        m_sliderAccel->Connect(wxEVT_SLIDER, wxCommandEventHandler(BKECtrl::OnSliderAccel), nullptr, this);
        m_buttonResetBoard->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                    wxCommandEventHandler(BKECtrl::OnClickResetBoard),
                                    nullptr,
                                    this);
        m_buttonHome->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                              wxCommandEventHandler(BKECtrl::OnClickGoToHome),
                              nullptr,
                              this);
        m_boardButtons[0]->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard00),
                                   nullptr,
                                   this);
        m_boardButtons[1]->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard01),
                                   nullptr,
                                   this);
        m_boardButtons[2]->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard02),
                                   nullptr,
                                   this);
        m_boardButtons[3]->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard03),
                                   nullptr,
                                   this);
        m_boardButtons[4]->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard04),
                                   nullptr,
                                   this);
        m_boardButtons[5]->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard05),
                                   nullptr,
                                   this);
        m_boardButtons[6]->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard06),
                                   nullptr,
                                   this);
        m_boardButtons[7]->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard07),
                                   nullptr,
                                   this);
        m_boardButtons[8]->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                                   wxCommandEventHandler(BKECtrl::OnClickBoard08),
                                   nullptr,
                                   this);

        m_strategyButtonManual->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
                                        wxCommandEventHandler(BKECtrl::OnToggleManual),
                                        nullptr,
                                        this);
        m_strategyButtonFair->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
                                      wxCommandEventHandler(BKECtrl::OnToggleFair),
                                      nullptr,
                                      this);
        m_strategyButtonEasy->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
                                      wxCommandEventHandler(BKECtrl::OnToggleEasy),
                                      nullptr,
                                      this);
    }
    void BKECtrl::DisconnectEvents()
    {
        m_fieldSpeed->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(BKECtrl::OnKillFocusSpeed), nullptr, this);
        m_fieldSpeed->Disconnect(wxEVT_COMMAND_TEXT_ENTER,
                                 wxCommandEventHandler(BKECtrl::OnTextEnterSpeed),
                                 nullptr,
                                 this);
        m_fieldSpeed->Disconnect(wxEVT_COMMAND_TEXT_MAXLEN,
                                 wxCommandEventHandler(BKECtrl::OnTextMaxLenSpeed),
                                 nullptr,
                                 this);
        m_sliderSpeed->Disconnect(wxEVT_SCROLL_THUMBRELEASE,
                                  wxScrollEventHandler(BKECtrl::OnSliderReleaseSpeed),
                                  nullptr,
                                  this);
        m_sliderSpeed->Disconnect(wxEVT_SLIDER, wxCommandEventHandler(BKECtrl::OnSliderSpeed), nullptr, this);
        m_fieldAccel->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(BKECtrl::OnKillFocusAccel), nullptr, this);
        m_fieldAccel->Disconnect(wxEVT_COMMAND_TEXT_ENTER,
                                 wxCommandEventHandler(BKECtrl::OnTextEnterAccel),
                                 nullptr,
                                 this);
        m_fieldAccel->Disconnect(wxEVT_COMMAND_TEXT_MAXLEN,
                                 wxCommandEventHandler(BKECtrl::OnTextMaxLenAccel),
                                 nullptr,
                                 this);
        m_sliderAccel->Disconnect(wxEVT_SCROLL_THUMBRELEASE,
                                  wxScrollEventHandler(BKECtrl::OnSliderReleaseAccel),
                                  nullptr,
                                  this);
        m_sliderAccel->Disconnect(wxEVT_SLIDER, wxCommandEventHandler(BKECtrl::OnSliderAccel), nullptr, this);
        m_buttonResetBoard->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                       wxCommandEventHandler(BKECtrl::OnClickResetBoard),
                                       nullptr,
                                       this);
        m_buttonHome->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                 wxCommandEventHandler(BKECtrl::OnClickGoToHome),
                                 nullptr,
                                 this);
        m_boardButtons[0]->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                      wxCommandEventHandler(BKECtrl::OnClickBoard00),
                                      nullptr,
                                      this);
        m_boardButtons[1]->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                      wxCommandEventHandler(BKECtrl::OnClickBoard01),
                                      nullptr,
                                      this);
        m_boardButtons[2]->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                      wxCommandEventHandler(BKECtrl::OnClickBoard02),
                                      nullptr,
                                      this);
        m_boardButtons[3]->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                      wxCommandEventHandler(BKECtrl::OnClickBoard03),
                                      nullptr,
                                      this);
        m_boardButtons[4]->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                      wxCommandEventHandler(BKECtrl::OnClickBoard04),
                                      nullptr,
                                      this);
        m_boardButtons[5]->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                      wxCommandEventHandler(BKECtrl::OnClickBoard05),
                                      nullptr,
                                      this);
        m_boardButtons[6]->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                      wxCommandEventHandler(BKECtrl::OnClickBoard06),
                                      nullptr,
                                      this);
        m_boardButtons[7]->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                      wxCommandEventHandler(BKECtrl::OnClickBoard07),
                                      nullptr,
                                      this);
        m_boardButtons[8]->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                                      wxCommandEventHandler(BKECtrl::OnClickBoard08),
                                      nullptr,
                                      this);

        m_strategyButtonManual->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
                                        wxCommandEventHandler(BKECtrl::OnToggleManual),
                                        nullptr,
                                        this);
        m_strategyButtonFair->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
                                      wxCommandEventHandler(BKECtrl::OnToggleFair),
                                      nullptr,
                                      this);
        m_strategyButtonEasy->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
                                      wxCommandEventHandler(BKECtrl::OnToggleEasy),
                                      nullptr,
                                      this);
    }
}// namespace Frame