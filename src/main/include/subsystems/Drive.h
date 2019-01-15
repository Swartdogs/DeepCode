/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "RobotMap.h"
#include "frc/WPILib.h"
#include <frc/commands/Subsystem.h>
#include "PIDControl.h"

class Drive : public frc::Subsystem {
 
 public:
  typedef enum {
    spLow,
    spHigh
  }ShifterPosition;

  Drive();
  void ArcadeDrive(double drive, double rotate);
  void SetShifter(ShifterPosition position);
  double RotateExec();
  void RotateInit(double heading, double maxSpeed, bool resetGyro);
  bool RotateIsFinished();
  void InitDefaultCommand() override;
  
private:
  PIDControl m_rotatePID{"Rotate"};

  ShifterPosition m_shifterPosition;

  frc::VictorSP m_driveLeft1{pwmDriveLeft1};
  frc::VictorSP m_driveLeft2{pwmDriveLeft2};
  frc::VictorSP m_driveRight1{pwmDriveRight1};
  frc::VictorSP m_driveRight2{pwmDriveRight2};

  frc::Encoder m_encoderLeft{dioEncoderLeftChannelA, dioEncoderLeftChannelB, false};
  frc::Encoder m_encoderRight{dioEncoderRightChannelA, dioEncoderRightChannelB, true};

  frc::ADXRS450_Gyro m_gyro{};

  frc::Solenoid m_solShifter{solShifter};
};