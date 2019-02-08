/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once
#include <frc/WPILib.h>
#include <frc/commands/Subsystem.h>
#include "RobotMap.h"
#include "PIDControl.h"

class Arm : public frc::Subsystem {
  public:
    typedef enum {
      apTravel,
      apPickup,
      apLow,
      apMid,
      apHigh,
      apCargoShip,
      apUnknown
    } ArmPosition;

    typedef enum {
      hmCargo,
      hmHatch
    } HandMode;

    typedef enum {
      hsRelease,
      hsGrab
    } HatchState;

    typedef enum{
      imOff,
      imIn,
      imOut,
      imRotate
    } IntakeMode;

    Arm();
    void InitDefaultCommand() override;    
    void Periodic() override;
    
    std::string   GetArmPositionName(ArmPosition position);
    bool          GetCargoDetected(); 
    HandMode      GetHandMode();
    std::string   GetHandModeName(HandMode mode);
    HatchState    GetHatchState();
    std::string   GetHatchStateName(HatchState state);
    IntakeMode    GetIntakeMode();
    double        GetShoulderDegrees();
    ArmPosition   GetShoulderPosition();
    double        GetShoulderSetpoint();
    double        GetWristDegrees();
    ArmPosition   GetWristPosition();
    double        GetWristSetpoint();
    bool          IsDrivenManually();
    void          SetDrivenManually(bool isManual);
    void          SetHandMode(HandMode mode);
    void          SetHatchState(HatchState state);
    void          SetIntakeMode(IntakeMode mode);
    void          SetShoulderPosition(ArmPosition position);
    void          SetShoulderPosition(double degrees, ArmPosition position = apUnknown);
    void          SetWristPosition(ArmPosition position);
    void          SetWristPosition(double degrees, ArmPosition position = apUnknown);
    bool          ShoulderAtSetpoint();
    bool          WristAtSetpoint();

  private:
    const double      SHOULDER_COUNTS_PER_DEGREE = 16.3;
    const double      WRIST_COUNTS_PER_DEGREE = 7;
    const int         EJECT_TIMEOUT = 50;                  //Sets to 20ms counts (1 sec = 50 counts)

    double            PowerLimit(double value);

    bool              m_manualDrive;
    double            m_shoulderSetpoint;
    double            m_wristSetpoint;

    ArmPosition       m_shoulderPosition;
    ArmPosition       m_wristPosition;
  
    HandMode          m_handMode;

    HatchState        m_hatchState;

    IntakeMode        m_intakeMode;

    frc::VictorSP     m_shoulderMotor{pwmShoulder};
    frc::VictorSP     m_wristMotor{pwmWrist};
    frc::VictorSP     m_handTop{pwmHandTop};
    frc::VictorSP     m_handBottom{pwmHandBottom};

    frc::DigitalInput m_cargoSensor{dioCargoSensor};

    frc::AnalogInput  m_shoulderPot{aioShoulderPot};
    frc::AnalogInput  m_wristPot{aioWristPot};

    PIDControl        m_shoulderPID{"Shoulder"};
    PIDControl        m_wristPID{"Wrist"};

    frc::Solenoid     m_solHand{solHand};
    frc::Solenoid     m_solHatch{solHatch};
};
