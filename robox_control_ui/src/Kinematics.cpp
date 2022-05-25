//
// Created by luke on 20-05-22.
// https://github.com/henriksod/Fabrik2DArduino
//

#include "Kinematics.h"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>

namespace Utils
{
    Kinematics::Kinematics(int aNumJoints, int* aLengths) : m_numJoints(aNumJoints), m_tolerance(1)
    {
        CreateChain(aLengths);
    }
    bool Kinematics::Solve(float x, float y, const int* lengths, int maxIter)
    {
        // Distance between root and target (root is always 0,0)
        int dist = static_cast<int>(std::sqrt(x * x + y * y));

        // Total length of chain
        int totalLength = 0;
        for (int i = 0; i < m_numJoints - 1; i++)
        {
            totalLength = totalLength + lengths[i];
        }

        // Check whether the target is within reach
        if (dist > totalLength)
        {
            // The target is unreachable

            for (int i = 0; i < m_numJoints - 1; i++)
            {
                // Find the distance r_i between the target (x,y) and the joint i position (jx,jy)
                float jx = m_chain->joints[i].x;
                float jy = m_chain->joints[i].y;
                float r_i = Distance(jx, jy, x, y);
                float lambda_i = ((float) lengths[i]) / r_i;

                // Find the new joint positions
                m_chain->joints[i + 1].x = (float) ((1 - lambda_i) * jx + lambda_i * x);
                m_chain->joints[i + 1].y = (float) ((1 - lambda_i) * jy + lambda_i * y);
            }

            return false;
        }
        else
        {
            // The target is reachable; this, set as (bx,by) the initial position of the joint i
            float bx = m_chain->joints[0].x;
            float by = m_chain->joints[0].y;

            // Check whether the distance between the end effector joint n (ex,ey) and the target is
            // greater than a tolerance
            float ex = m_chain->joints[m_numJoints - 1].x;
            float ey = m_chain->joints[m_numJoints - 1].y;
            float dif = Distance(ex, ey, x, y);

            float prevDif = 0;
            float tolerance = m_tolerance;
            int iterationCounter = 0;


            while (dif > tolerance)
            {
                if (prevDif == dif)
                    tolerance *= 2;

                prevDif = dif;

                m_chain->joints[m_numJoints - 1].x = x;
                m_chain->joints[m_numJoints - 1].y = y;

                for (int i = m_numJoints - 2; i >= 0; i--)
                {
                    float jx = m_chain->joints[i].x;
                    float jy = m_chain->joints[i].y;
                    float nx = m_chain->joints[i + 1].x;
                    float ny = m_chain->joints[i + 1].y;
                    float r_i = Distance(jx, jy, nx, ny);
                    float lambda_i = ((float) lengths[i]) / r_i;

                    // Find the new joint positions
                    m_chain->joints[i].x = (float) ((1 - lambda_i) * nx + lambda_i * jx);
                    m_chain->joints[i].y = (float) ((1 - lambda_i) * ny + lambda_i * jy);
                }

                // STAGE 2: BACKWARD REACHING
                // Set the root at its initial position
                m_chain->joints[0].x = bx;
                m_chain->joints[0].y = by;

                for (int i = 0; i < m_numJoints - 1; i++)
                {
                    // Find the distance r_i between the new joint position i (nx,ny)
                    // and the joint i+1 (jx,jy)
                    float jx = m_chain->joints[i + 1].x;
                    float jy = m_chain->joints[i + 1].y;
                    float nx = m_chain->joints[i].x;
                    float ny = m_chain->joints[i].y;
                    float r_i = Distance(jx, jy, nx, ny);
                    float lambda_i = ((float) lengths[i]) / r_i;

                    // Find the new joint positions
                    m_chain->joints[i + 1].x = (float) ((1 - lambda_i) * nx + lambda_i * jx);
                    m_chain->joints[i + 1].y = (float) ((1 - lambda_i) * ny + lambda_i * jy);
                }

                // Update distance between end effector and target
                ex = m_chain->joints[m_numJoints - 1].x;
                ey = m_chain->joints[m_numJoints - 1].y;
                dif = Distance(ex, ey, x, y);

                ++iterationCounter;
                if (iterationCounter >= maxIter)
                {
                    // permitted amount of iterations exceeded.
                    return false;
                }
            }
        }

        m_chain->joints[0].angle = std::atan2(m_chain->joints[1].y, m_chain->joints[1].x);

        float prevAngle = m_chain->joints[0].angle;
        for (int i = 2; i <= m_numJoints - 1; i++)
        {
            float ax = m_chain->joints[i - 1].x;
            float ay = m_chain->joints[i - 1].y;
            float bx = m_chain->joints[i].x;
            float by = m_chain->joints[i].y;

            float aAngle = std::atan2(by - ay, bx - ax);

            m_chain->joints[i - 1].angle = aAngle - prevAngle;

            prevAngle = aAngle;
        }

        return true;
    }
    float Kinematics::GetX(int joint)
    {
        if (joint >= 0 && joint < m_numJoints)
        {
            return m_chain->joints[joint].x;
        }
        return 0;
    }
    float Kinematics::GetY(int joint)
    {
        if (joint >= 0 && joint < m_numJoints)
        {
            return m_chain->joints[joint].y;
        }
        return 0;
    }
    float Kinematics::GetAngle(int joint)
    {
        if (joint >= 0 && joint < m_numJoints)
        {
            return m_chain->joints[joint].angle;
        }
        return 0;
    }
    void Kinematics::SetTolerance(float tolerance)
    {
        m_tolerance = tolerance;
    }
    void Kinematics::SetJoints(const float* angles, const int* lengths)
    {
        float accAng = angles[0];
        float accX = 0;
        float accY = 0;
        m_chain->joints[0].angle = angles[0];

        for (int i = 1; i < m_numJoints; i++)
        {
            accAng += angles[i];
            m_chain->joints[i].x = accX + (float) lengths[i - 1] * std::cos(accAng);
            m_chain->joints[i].y = accY + (float) lengths[i - 1] * std::sin(accAng);
            m_chain->joints[i].angle = angles[i];
            accX = m_chain->joints[i].x;
            accY = m_chain->joints[i].y;
        }
    }
    float Kinematics::Distance(float x1, float y1, float x2, float y2)
    {
        float xDiff = x2 - x1;
        float yDiff = y2 - y1;
        return std::sqrt(xDiff * xDiff + yDiff * yDiff);
    }
    void Kinematics::CreateChain(const int* lengths)
    {
        auto* chain = (Chain*) malloc(sizeof(Chain));
        chain->joints = (Joint*) malloc(sizeof(Joint) * static_cast<uint32_t>(m_numJoints));

        chain->joints[0].x = 0;
        chain->joints[0].y = 0;
        chain->joints[0].angle = 0;

        int sumLengths = 0;
        for (int i = 1; i < m_numJoints; i++)
        {
            sumLengths = sumLengths + lengths[i - 1];
            chain->joints[i].x = 0;
            chain->joints[i].y = static_cast<float>(sumLengths);
            chain->joints[i].angle = 0;
        }

        m_chain = chain;
    }

}// namespace Utils