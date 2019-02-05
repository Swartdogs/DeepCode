#pragma once

#include <frc/WPILib.h>
#include "RobotMap.h"

class OI {
 public:
  OI();
  
  double ApplyDeadband(double joystickValue, double deadband);
  double GetDriveJoystickX();
  double GetDriveJoystickY();
  
 private:
  frc::Joystick  m_driveJoystick{0};
};
