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

  typedef enum {
    ueLeftEncoder,
    ueRightEncoder,
    ueBothEncoders,
    ueCurrentEncoder
  }UseEncoder;
  
  Drive();
  ~Drive();

  void              InitDefaultCommand() override;

  void              ArcadeDrive(double drive, double rotate);
  double            DriveExec();
  void              DriveInit(double distance, double heading, double maxSpeed, double minSpeed,
                              bool resetEncoder, bool resetGyro, bool driveArc = false);
  bool              DriveIsFinished();
  double            GetDistance (UseEncoder encoder);
  double            GetHeading(); 
  ShifterPosition   GetShifterPosition(); 
  const char*       GetShifterPositionName(ShifterPosition position);
  double            RotateExec();
  void              RotateInit(double heading, double maxSpeed, bool resetGyro);
  bool              RotateIsFinished();
  void              SetDriveEnable(bool enable); 
  void              SetShifter(ShifterPosition position);

private:
  bool                m_driveEnable; 

  frc::ADXRS450_Gyro  m_gyro{};

  frc::Encoder        m_encoderLeft{dioEncoderLeftChannelA, dioEncoderLeftChannelB, false};
  frc::Encoder        m_encoderRight{dioEncoderRightChannelA, dioEncoderRightChannelB, true};

  PIDControl          m_drivePID{"Drive"};
  PIDControl          m_rotatePID{"Rotate"};

  ShifterPosition     m_shifterPosition;

  frc::Solenoid       m_solShifter{solShifter};

  UseEncoder          m_useEncoder;

  frc::VictorSP       m_driveLeft1{pwmDriveLeft1};
  frc::VictorSP       m_driveLeft2{pwmDriveLeft2};
  frc::VictorSP       m_driveRight1{pwmDriveRight1};
  frc::VictorSP       m_driveRight2{pwmDriveRight2};
};