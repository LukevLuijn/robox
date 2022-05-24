//
// Created by luke on 14-05-22.
//

#ifndef ROBOC_UI_DATA_MANAGER_H
#define ROBOC_UI_DATA_MANAGER_H

#include <cstdint>
#include <vector>

#include "Point.h"

namespace Driver
{
    typedef Utils::Point <float, 3> Position;

    struct SegmentData
    {
        //=================== static data
        uint16_t m_stepPin{};
        uint16_t m_directionPin{};
        uint16_t m_enablePin{};
        uint16_t m_hallSensorPin{};
        uint16_t m_microStepping{};
        int32_t m_maxPosition{};
        int32_t m_minPosition{};
        uint16_t m_maxSpeed{};
        uint16_t m_maxAccel{};
        float m_stepsPerUnit{};
        //=================== dynamic data
        float m_idlePosition{};
        float m_currentSpeed{};
        float m_currentAccel{};
        //=================== runTime data
        float m_currentPosition{};
        bool m_IsActive{};
    };

    struct DataManager
    {
    public:
        SegmentData m_segment00{};
        SegmentData m_segment01{};
        SegmentData m_segment02{};
        SegmentData m_segment03{};

        Position m_gripperPosition;

        uint16_t m_gripperPin00{};
        uint16_t m_gripperPin01{};

        uint16_t m_gripperPWM00{};
        uint16_t m_gripperPWM01{};

        uint16_t m_lengthSegment01{};
        uint16_t m_lengthSegment02{};
        uint16_t m_leadScrewPitch{};

        bool m_verbosityError{};
        bool m_verbosityWarning{};
        bool m_verbosityInfo{};
        bool m_verbosityDebug{};

        float m_frameHeightOffset{};
        float m_gripperHeightOffset{};
        float m_actualZeroPosSeg00{};

    public:
        /**
         * @brief Parse incoming message
         * 
         * @param message
         */
        void ParseMessage(const std::string& message);
        /**
         * @brief Print contents of the instance.
         * 
         */
        [[maybe_unused]] void Print();

    private:
        void ParseRunTimeData(const std::vector<std::string>& data);
        void ParseDynamicData(const std::vector<std::string>& data);
        void ParseStaticData(const std::vector<std::string>& data);
    };
}

#endif//ROBOC_UI_DATA_MANAGER_H
