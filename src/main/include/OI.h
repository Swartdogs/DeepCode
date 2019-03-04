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

  frc::JoystickButton m_buttonDrive1{&m_driveJoystick, 1};
  frc::JoystickButton m_buttonDrive2{&m_driveJoystick, 2};
  frc::JoystickButton m_buttonDrive6{&m_driveJoystick, 6};
  frc::JoystickButton m_buttonDrive7{&m_driveJoystick, 7};
  frc::JoystickButton m_buttonDrive8{&m_driveJoystick, 8};
  frc::JoystickButton m_buttonDrive10{&m_driveJoystick, 10};

  frc::JoystickButton m_buttonArm1{&m_armJoystick, 1};
  frc::JoystickButton m_buttonArm6{&m_armJoystick, 6};
  frc::JoystickButton m_buttonArm7{&m_armJoystick, 7};
  frc::JoystickButton m_buttonArm8{&m_armJoystick, 8};
  frc::JoystickButton m_buttonArm9{&m_armJoystick, 9};
  frc::JoystickButton m_buttonArm10{&m_armJoystick, 10};
  frc::JoystickButton m_buttonArm11{&m_armJoystick, 11};
 
  frc::JoystickButton m_buttonBox1{&m_buttonBox, 1};
  frc::JoystickButton m_buttonBox2{&m_buttonBox, 2};
  frc::JoystickButton m_buttonBox3{&m_buttonBox, 3};
  frc::JoystickButton m_buttonBox4{&m_buttonBox, 4};
  frc::JoystickButton m_buttonBox5{&m_buttonBox, 5};
  frc::JoystickButton m_buttonBox6{&m_buttonBox, 6};
  frc::JoystickButton m_buttonBox7{&m_buttonBox, 7};
  frc::JoystickButton m_buttonBox8{&m_buttonBox, 8};
  frc::JoystickButton m_buttonBox9{&m_buttonBox, 9};
  frc::JoystickButton m_buttonBox10{&m_buttonBox, 10};
  frc::JoystickButton m_buttonBox11{&m_buttonBox, 11};
  frc::JoystickButton m_buttonBox12{&m_buttonBox, 12};

  frc::InternalButton m_InternalLevel2;
  frc::InternalButton m_InternalLevel3;
  frc::InternalButton m_InternalHandMode;
};
