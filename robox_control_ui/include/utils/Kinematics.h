// ======================================
// Filename:    Kinematics.h
// Project:     ROBOX Control UI
// Author:      Luke van Luijn
// Data:        20-05-2022
// ======================================

#ifndef ROBOX_CONTROL_UI_KINEMATICS_H
#define ROBOX_CONTROL_UI_KINEMATICS_H

namespace Utils
{
    class Kinematics
    {
    private:
        typedef struct {
            float x;
            float y;
            float angle;
        } Joint;
        typedef struct {
            Joint* joints;
            float z;
            float angle;
        } Chain;

    public:
        Kinematics(int numJoints, int* lengths);
        /**
         * @brief Solve inverse kinematics for given position.
         * 
         * @param x Target
         * @param y Target
         * @param lengths Lenghts of the robot.
         * @param maxIter Max amount of iterations to solve.
         * @return true If a solution was found.
         * @return false If no solution was found.
         */
        bool Solve(float x, float y, const int* lengths, int maxIter = 1000);
        /**
         * @brief Get x position of given joint.
         * 
         * @param joint Index of requested joint.
         * @return float x-position of joint.
         */
        float GetX(int joint);
        /**
         * @brief Get y position of given joint.
         * 
         * @param joint Index of requested joint.
         * @return float y-position of joint.
         */
        float GetY(int joint);
        /**
         * @brief Get the angle of given joint.
         * 
         * @param joint Index of requested joint.
         * @return float angle of joint, in radians.
         */
        float GetAngle(int joint);
        /**
         * @brief Set the tolerance for solving.
         * 
         * @param tolerance 
         */
        void SetTolerance(float tolerance);
        /**
         * @brief Set the joints and solve with forward kinematics.
         * 
         * @param angles Angles of bones.
         * @param length Length of bones.
         */
        void SetJoints(const float* angles, const int* length);

    private:
        static float Distance(float x1, float y1, float x2, float y2);
        void CreateChain(const int* lengths);

    private:
        int m_numJoints;
        float m_tolerance;
        Chain* m_chain{};
    };
}// namespace Utils

#endif//ROBOX_CONTROL_UI_KINEMATICS_H