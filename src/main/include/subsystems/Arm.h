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
      apCargoShip
    } ArmPosition;

    Arm();
    void InitDefaultCommand() override;    
    void Periodic() override;
    
    bool          GetArmInUse(); 
    std::string   GetArmPositionName(ArmPosition position);
    double        GetShoulderPosition();
    double        GetWristPosition();
    void          SetArmInUse(bool inUse);
    void          SetShoulderPosition(ArmPosition position);
    void          SetShoulderPosition(double position);
    void          SetWristPosition(ArmPosition position);
    void          SetWristPosition(double position);
    bool          ShoulderAtSetpoint();
    bool          WristAtSetpoint();

  private:
    const double      SHOULDER_COUNTS_PER_DEGREE = 16.3;
    const double      WRIST_COUNTS_PER_DEGREE = 7;

    bool              m_armInUse;

    frc::VictorSP     m_shoulderMotor{pwmShoulder};
    frc::VictorSP     m_wristMotor{pwmWrist};

    frc::AnalogInput  m_shoulderPot{aioShoulderPot};
    frc::AnalogInput  m_wristPot{aioWristPot};

    PIDControl        m_shoulderPID{"Shoulder"};
    PIDControl        m_wristPID{"Wrist"};

};
