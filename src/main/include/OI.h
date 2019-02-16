#pragma once

#include <frc/WPILib.h>
#include "RobotMap.h"

class OI {
 public:
  OI();
  
  double  ApplyDeadband(double joystickValue, double deadband);
  double  GetArmJoystickX();
  double  GetArmJoystickY();
  double  GetDriveJoystickX();
  double  GetDriveJoystickY();
  bool    InHatchMode();
  void    Periodic(); 
  
 private:
  frc::Joystick  m_driveJoystick{0};
  frc::Joystick  m_armJoystick{1};
  frc::Joystick  m_buttonBox{2};

  frc::JoystickButton m_buttonShifter{&m_driveJoystick, 1};
  frc::JoystickButton m_buttonClimbEnable{&m_driveJoystick, 9};
  frc::JoystickButton m_buttonClimbCancel{&m_driveJoystick, 10};
  //frc::JoystickButton m_buttonClimbLevel2{&m_buttonBox, 1};
  //frc::JoystickButton m_buttonClimbLevel3{&m_buttonBox, 2};
 
  frc::JoystickButton m_buttonIntakeIn{&m_buttonBox, 1};
  frc::JoystickButton m_buttonIntakeOut{&m_buttonBox, 2};
  frc::JoystickButton m_buttonIntakeRotate{&m_buttonBox, 3};
  frc::JoystickButton m_buttonGrabHatch{&m_buttonBox, 5};
  frc::JoystickButton m_buttonReleaseHatch{&m_buttonBox, 6};

  frc::InternalButton m_InternalLevel2;
  frc::InternalButton m_InternalLevel3;
  frc::InternalButton m_InternalHandMode;
};
