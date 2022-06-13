// ======================================
// Filename:    Robot.cpp
// Project:     ROBOX Motor controller
// Author:      Luke van Luijn
// Data:        14-05-2022
// ======================================

#include "base/Robot.h"
#include "utils/Logger.h"

namespace Base
{
    Robot::Robot()
        : Z0(Device::LinearActuator(MOTOR_STP_0, MOTOR_DIR_0, MOTOR_ENB_0, HALL_SENSOR_0,
                                    Device::Microstepping_e::MS3200, 1.0f, false)),
          A1(Device::AxialActuator(MOTOR_STP_1, MOTOR_DIR_1, MOTOR_ENB_1, HALL_SENSOR_1,
                                   Device::Microstepping_e::MS3200, 7.2f, true, Device::HomingStyle_e::DOUBLE_MAGNET)),
          A2(Device::AxialActuator(MOTOR_STP_2, MOTOR_DIR_2, MOTOR_ENB_2, HALL_SENSOR_2,
                                   Device::Microstepping_e::MS3200, 7.2f, false, Device::HomingStyle_e::SINGLE_MAGNET)),
          A3(Device::AxialActuator(MOTOR_STP_3, MOTOR_DIR_3, MOTOR_ENB_3, HALL_SENSOR_3,
                                   Device::Microstepping_e::MS3200, 7.2f, false, Device::HomingStyle_e::SINGLE_MAGNET))
    {
    }
    void Robot::Start()
    {
        Z0.Start();
        DEBUG("Setup complete", "Linear actuator A0");

        A1.Start();
        DEBUG("Setup complete", "Linear actuator A0");

        A2.Start();
        DEBUG("Setup complete", "Linear actuator A0");

        A3.Start();
        DEBUG("Setup complete", "Linear actuator A0");

        pinMode(GRIPPER_SIGNAL_00, OUTPUT);
        pinMode(GRIPPER_SIGNAL_01, OUTPUT);
        DEBUG("Setup complete", "Gripper");

        DEBUG("Setup complete", "Robot");
    }
    void Robot::Run()
    {        
        Z0.Run();
        A1.Run();
        A2.Run();
        A3.Run();
    }
    void Robot::SetTarget(const Position& target)
    {
        SetTarget(Translate(target));
    }
    void Robot::SetTarget(const Angles& target)
    {
        if (CheckBounds(target))
        {
            Z0.MoveToHeight(target.Z0);
            A1.MoveToAngle(target.A1);
            A2.MoveToAngle(target.A2);
            A3.MoveToAngle(target.A3);


            String targetZ0 = "0:" + String(target.Z0);
            String targetA1 = "1:" + String(target.A1);
            String targetA2 = "2:" + String(target.A2);
            String targetA3 = "3:" + String(target.A3);
            String varString = targetZ0 + "," + targetA1 + "," + targetA2 + "," + targetA3;

            DEBUG("New target set", varString);
        }
    }
    void Robot::DisableRobot()
    {
        Z0.disableOutputs();
        A1.disableOutputs();
        A2.disableOutputs();
        A3.disableOutputs();

        SetGripperPWMSignal00(0);
        SetGripperPWMSignal01(0);

        INFO("Robot disabled");
    }
    void Robot::DisableJoint(Joint_e joint)
    {
        switch (joint)
        {
            case Joint_e::Z0:
                Z0.disableOutputs();
                break;
            case Joint_e::A1:
                A1.disableOutputs();
                break;
            case Joint_e::A2:
                A2.disableOutputs();
                break;
            case Joint_e::A3:
                A3.disableOutputs();
                break;
        }
        INFO("Joint disabled", String((uint16_t) joint));
    }
    void Robot::EnableRobot()
    {
        Z0.enableOutputs();
        A1.enableOutputs();
        A2.enableOutputs();
        A3.enableOutputs();

        INFO("Robot enabled");
    }
    void Robot::EnableJoint(Joint_e joint)
    {
        switch (joint)
        {
            case Joint_e::Z0:
                Z0.enableOutputs();
                break;
            case Joint_e::A1:
                A1.enableOutputs();
                break;
            case Joint_e::A2:
                A2.enableOutputs();
                break;
            case Joint_e::A3:
                A3.enableOutputs();
                break;
        }
        INFO("Joint enabled", String((uint16_t) joint));
    }
    void Robot::HomingSequence()
    {
        Z0.HomingSequence(HOMING_SPEED_SEGMENT_00, HOMING_ACCEL_SEGMENT_00, MAX_SPEED_SEGMENT_00, MAX_ACCEL_SEGMENT_00);
        A1.HomingSequence(HOMING_SPEED_SEGMENT_01, HOMING_ACCEL_SEGMENT_01, MAX_SPEED_SEGMENT_01, MAX_ACCEL_SEGMENT_01);
        A2.HomingSequence(HOMING_SPEED_SEGMENT_02, HOMING_ACCEL_SEGMENT_02, MAX_SPEED_SEGMENT_02, MAX_ACCEL_SEGMENT_02);
        A3.HomingSequence(HOMING_SPEED_SEGMENT_03, HOMING_ACCEL_SEGMENT_03, MAX_SPEED_SEGMENT_03, MAX_ACCEL_SEGMENT_03);
    }
    void Robot::ResetHoming()
    {
        Z0.ResetHoming();
        A1.ResetHoming();
        A2.ResetHoming();
        A3.ResetHoming();
        DEBUG("Homing sequence reset");
    }
    void Robot::SetGripperPWMSignal00(uint8_t signal)
    {
        m_valueGripper00 = signal;
        analogWrite(GRIPPER_SIGNAL_00, m_valueGripper00);

        DEBUG("PWM value set Gripper 00", String((uint16_t) signal));
    }
    void Robot::SetGripperPWMSignal01(uint8_t signal)
    {
        m_valueGripper01 = signal;
        analogWrite(GRIPPER_SIGNAL_01, m_valueGripper01);

        DEBUG("PWM value set Gripper 01", String((uint16_t) signal));
    }
    uint8_t Robot::GetGripperPWMSignal00() const
    {
        return m_valueGripper00;
    }
    uint8_t Robot::GetGripperPWMSignal01() const
    {
        return m_valueGripper01;
    }
    Point Robot::GetPosition()
    {
        Angles angles{
                Z0.GetCurrentHeight(),
                A1.GetCurrentAngle(),
                A2.GetCurrentAngle(),
                A3.GetCurrentAngle(),
        };
        Position position = Translate(angles);
        return Point{position.X, position.Y, position.Z};
    }
    float Robot::GetJointPosition(Joint_e joint)
    {
        switch (joint)
        {
            case Joint_e::Z0:
                return Z0.GetCurrentHeight();
                break;
            case Joint_e::A1:
                return A1.GetCurrentAngle();
                break;
            case Joint_e::A2:
                return A2.GetCurrentAngle();
                break;
            case Joint_e::A3:
                return A3.GetCurrentAngle();
                break;
            default:
                ERROR("Undefined joint, should never happen", String((uint16_t) joint));
                return 0.0f;
                break;
        }
    }
    bool Robot::IsRunning(Joint_e joint)
    {
        switch (joint)
        {
            case Joint_e::Z0:
                return Z0.isRunning();
                break;
            case Joint_e::A1:
                return A1.isRunning();
                break;
            case Joint_e::A2:
                return A2.isRunning();
                break;
            case Joint_e::A3:
                return A3.isRunning();
                break;
            default:
                ERROR("Undefined joint, should never happen", String((uint16_t) joint));
                return false;
                break;
        }
    }
    bool Robot::IsRunning()
    {
        return Z0.isRunning() && A1.isRunning() && A2.isRunning() && A3.isRunning();
    }
    bool Robot::IsMoveComplete()
    {
        return (Z0.distanceToGo() == 0) && (A1.distanceToGo() == 0) && (A2.distanceToGo() == 0) &&
               (A3.distanceToGo() == 0);
    }
    bool Robot::IsHomingComplete()
    {
        return Z0.HomingComplete() && A1.HomingComplete() && A2.HomingComplete() && A3.HomingComplete();
    }
    void Robot::SetSpeed(float speed)
    {
        Z0.setMaxSpeed(speed);
        A1.setMaxSpeed(speed);
        A2.setMaxSpeed(speed);
        A3.setMaxSpeed(speed);
    }
    void Robot::SetAccel(float accel)
    {
        Z0.SetAcceleration(accel);
        A1.SetAcceleration(accel);
        A2.SetAcceleration(accel);
        A3.SetAcceleration(accel);
    }
    void Robot::SetSpeed(Joint_e joint, float speed)
    {
        switch (joint)
        {
            case Joint_e::Z0:
                Z0.setMaxSpeed(speed);
                break;
            case Joint_e::A1:
                A1.setMaxSpeed(speed);
                break;
            case Joint_e::A2:
                A2.setMaxSpeed(speed);
                break;
            case Joint_e::A3:
                A3.setMaxSpeed(speed);
                break;
        }
    }
    void Robot::SetAccel(Joint_e joint, float accel)
    {
        switch (joint)
        {
            case Joint_e::Z0:
                Z0.SetAcceleration(accel);
                break;
            case Joint_e::A1:
                A1.SetAcceleration(accel);
                break;
            case Joint_e::A2:
                A2.SetAcceleration(accel);
                break;
            case Joint_e::A3:
                A3.SetAcceleration(accel);
                break;
        }
    }
    float Robot::GetSpeed(Joint_e joint)
    {
        switch (joint)
        {
            case Joint_e::Z0:
                return Z0.maxSpeed();
                break;
            case Joint_e::A1:
                return A1.maxSpeed();
                break;
            case Joint_e::A2:
                return A2.maxSpeed();
                break;
            case Joint_e::A3:
                return A3.maxSpeed();
                break;
            default:
                ERROR("Undefined joint, should never happen", String((uint16_t) joint));
                return 0.0f;
                break;
        }
    }
    float Robot::GetAccel(Joint_e joint)
    {
        switch (joint)
        {
            case Joint_e::Z0:
                return Z0.GetAcceleration();
                break;
            case Joint_e::A1:
                return A1.GetAcceleration();
                break;
            case Joint_e::A2:
                return A2.GetAcceleration();
                break;
            case Joint_e::A3:
                return A3.GetAcceleration();
                break;
            default:
                ERROR("Undefined joint, should never happen", String((uint16_t) joint));
                return 0.0f;
                break;
        }
    }
    Device::Microstepping_e Robot::GetMicrostepping(Joint_e joint)
    {
        switch (joint)
        {
            case Joint_e::Z0:
                return Z0.GetMicrostepping();
                break;
            case Joint_e::A1:
                return A1.GetMicrostepping();
                break;
            case Joint_e::A2:
                return A2.GetMicrostepping();
                break;
            case Joint_e::A3:
                return A3.GetMicrostepping();
                break;
            default:
                ERROR("Undefined joint, should never happen", String((uint16_t) joint));
                return (Device::Microstepping_e) 0;
                break;
        }
    }
    float Robot::GetStepsPerType(Joint_e joint)
    {
        switch (joint)
        {
            case Joint_e::Z0:
                return Z0.GetStepsPerMillimeter();
                break;
            case Joint_e::A1:
                return A1.GetStepsPerDegree();
                break;
            case Joint_e::A2:
                return A2.GetStepsPerDegree();
                break;
            case Joint_e::A3:
                return A3.GetStepsPerDegree();
                break;
            default:
                ERROR("Undefined joint, should never happen", String((uint16_t) joint));
                return 0.0f;
                break;
        }
    }
    Angles Robot::Translate(Position target)
    {
        // https://howtomechatronics.com/projects/scara-robot-how-to-build-your-own-arduino-based-robot/

        float theta1, theta2, phi;

        theta2 = acos((sq(target.X) + sq(target.Y) - sq(LENGTH_SEGMENT_01) - sq(LENGTH_SEGMENT_02)) /
                      (2 * LENGTH_SEGMENT_01 * LENGTH_SEGMENT_02));
        if (target.X < 0 && target.Y < 0)
        {
            theta2 = (-1) * theta2;
        }

        theta1 = atan(target.X / target.Y) -
                 atan((LENGTH_SEGMENT_02 * sin(theta2)) / (LENGTH_SEGMENT_01 + LENGTH_SEGMENT_02 * cos(theta2)));

        theta2 = (-1) * theta2 * 180 / PI;
        theta1 = theta1 * 180 / PI;

        // Angles adjustment depending in which quadrant the final tool coordinate x,y is
        if (target.X >= 0 && target.Y >= 0)
        {// 1st quadrant
            theta1 = 90 - theta1;
        }
        if (target.X < 0 && target.Y > 0)
        {// 2nd quadrant
            theta1 = 90 - theta1;
        }
        if (target.X < 0 && target.Y < 0)
        {// 3d quadrant
            theta1 = 270 - theta1;
            phi = 270 - theta1 - theta2;
            phi = (-1) * phi;
        }
        if (target.X > 0 && target.Y < 0)
        {// 4th quadrant
            theta1 = -90 - theta1;
        }
        if (target.X < 0 && target.Y == 0)
        {
            theta1 = 270 + theta1;
        }

        // Calculate "phi" angle so gripper is parallel to the X axis
        phi = 90 + theta1 + theta2;
        phi = (-1) * phi;

        // Angle adjustment depending in which quadrant the final tool coordinate x,y is
        if (target.X < 0 && target.Y < 0)
        {// 3d quadrant
            phi = 270 - theta1 - theta2;
        }
        if (abs(phi) > 165)
        {
            phi = 180 + phi;
        }

        return Angles{target.Z, theta1, theta2, phi};
    }
    Position Robot::Translate(Angles target)
    {
        float theta1F = A1.GetCurrentAngle() * PI / 180;// degrees to radians
        float theta2F = A2.GetCurrentAngle() * PI / 180;
        float xP = round(LENGTH_SEGMENT_01 * cos(theta1F) + LENGTH_SEGMENT_02 * cos(theta1F + theta2F));
        float yP = round(LENGTH_SEGMENT_01 * sin(theta1F) + LENGTH_SEGMENT_02 * sin(theta1F + theta2F));

        return Position{xP, yP, Z0.GetCurrentHeight(), A3.GetCurrentAngle()};
    }
    bool Robot::CheckBounds(const Angles& target)
    {
        bool whitinBounds = (target.Z0 >= MIN_HEIGHT_SEGMENT_00 && target.Z0 <= ACTUAL_ZERO_POS_SEG_00) &&
                            (target.A1 >= MIN_ANGLE_SEGMENT_01 && target.A1 <= MAX_ANGLE_SEGMENT_01) &&
                            (target.A2 >= MIN_ANGLE_SEGMENT_02 && target.A2 <= MAX_ANGLE_SEGMENT_02) &&
                            (target.A3 >= MIN_ANGLE_SEGMENT_03 && target.A3 <= MAX_ANGLE_SEGMENT_03);

        if (!whitinBounds)
        {
            String targetZ0 = "0:" + String(target.Z0);
            String targetA1 = "1:" + String(target.A1);
            String targetA2 = "2:" + String(target.A2);
            String targetA3 = "3:" + String(target.A3);
            String varString = targetZ0 + "," + targetA1 + "," + targetA2 + "," + targetA3;
            
            WARNING("Requested move out of bounds, move ignored", varString);
        }
        return whitinBounds;
    }
}// namespace Base