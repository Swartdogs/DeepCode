#pragma once

#include <frc/WPILib.h>
#include "RobotMap.h"

class OI {
 public:
  OI();
  
  double ApplyDeadband(double joystickValue, double deadband);
  double GetArmJoystickX();
  double GetArmJoystickY();
  
 private:
  frc::Joystick  m_armJoystick{1};
};
