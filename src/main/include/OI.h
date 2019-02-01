/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc/WPILib.h>
#include "RobotMap.h"

class OI {
 public:
  OI();
  
  double ApplyDeadband(double joystickValue, double deadband);
  double GetDriveJoystickX();
  double GetDriveJoystickY();
  void Periodic(); 
  
 private:
  frc::Joystick  m_driveJoystick{0};
  frc::Joystick  m_buttonBox{1};

  frc::JoystickButton m_buttonClimbEnable{&m_driveJoystick, 11};
  frc::JoystickButton m_buttonClimbCancel{&m_driveJoystick, 12};
  frc::JoystickButton m_buttonClimbLevel2{&m_buttonBox, 1};
  frc::JoystickButton m_buttonClimbLevel3{&m_buttonBox, 2};
 

  frc::InternalButton m_InternalLevel2;
  frc::InternalButton m_InternalLevel3;
};
