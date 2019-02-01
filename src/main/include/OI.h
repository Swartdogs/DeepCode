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
  double GetArmJoystickX();
  double GetArmJoystickY();
  double GetDriveJoystickX();
  double GetDriveJoystickY();
  
 private:
  frc::Joystick  m_driveJoystick{0};
  frc::Joystick  m_armJoystick{1};
};
