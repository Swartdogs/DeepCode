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
//  frc::Joystick  m_armJoystick{1};
  frc::Joystick  m_buttonBox{2};

  frc::JoystickButton m_buttonShifter{&m_driveJoystick, 1};
  frc::JoystickButton m_buttonClimbEnable{&m_driveJoystick, 9};
  frc::JoystickButton m_buttonClimbCancel{&m_driveJoystick, 10};
  //frc::JoystickButton m_buttonClimbLevel2{&m_buttonBox, 1};
  //frc::JoystickButton m_buttonClimbLevel3{&m_buttonBox, 2};
 
  frc::JoystickButton m_button1{&m_buttonBox, 1};
  frc::JoystickButton m_button2{&m_buttonBox, 2};
  frc::JoystickButton m_button3{&m_buttonBox, 3};
  frc::JoystickButton m_button4{&m_buttonBox, 4};
  frc::JoystickButton m_button5{&m_buttonBox, 5};
  frc::JoystickButton m_button6{&m_buttonBox, 6};
  frc::JoystickButton m_button7{&m_buttonBox, 7};
  frc::JoystickButton m_button8{&m_buttonBox, 8};
  frc::JoystickButton m_button9{&m_buttonBox, 9};
  frc::JoystickButton m_button10{&m_buttonBox, 10};
  frc::JoystickButton m_button12{&m_buttonBox, 12};

  frc::InternalButton m_InternalLevel2;
  frc::InternalButton m_InternalLevel3;
  frc::InternalButton m_InternalHandMode;
};
