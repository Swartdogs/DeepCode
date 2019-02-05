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
  void Periodic(); 
  
 private:
  frc::Joystick  m_driveJoystick{0};
  frc::Joystick  m_armJoystick{1};
  frc::Joystick  m_buttonBox{2};

  frc::JoystickButton m_buttonClimbEnable{&m_driveJoystick, 11};
  frc::JoystickButton m_buttonClimbCancel{&m_driveJoystick, 12};
  frc::JoystickButton m_buttonClimbLevel2{&m_buttonBox, 1};
  frc::JoystickButton m_buttonClimbLevel3{&m_buttonBox, 2};
 

  frc::InternalButton m_InternalLevel2;
  frc::InternalButton m_InternalLevel3;
};
