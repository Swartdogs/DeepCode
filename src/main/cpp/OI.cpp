/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "OI.h"
#include <frc/WPILib.h>
#include "commands/GrpClimb.h"
#include "commands/CmdCancelClimb.h"
#include "subsystems/Elevator.h"

OI::OI() {
  m_InternalLevel2.WhenPressed    (new GrpClimb(Elevator::epLevel2Extended));
  m_InternalLevel3.WhenPressed    (new GrpClimb(Elevator::epLevel3Extended));
  m_buttonClimbCancel.WhenPressed (new CmdCancelClimb());
  // Process operator interface input here.
}

double OI::ApplyDeadband(double joystickValue, double deadband) {
  deadband = fabs(deadband);

  if (joystickValue > deadband) {
    return ((joystickValue - deadband) / (1 - deadband));
  } else if (joystickValue < -deadband) {
    return ((joystickValue + deadband) / (1 - deadband));
  }
  
  return 0;
}

double OI::GetDriveJoystickX(){
  return m_driveJoystick.GetX();
}

double OI::GetDriveJoystickY(){
  return -m_driveJoystick.GetY();
}

void OI::Periodic() {
  m_InternalLevel2.SetPressed(m_buttonClimbEnable.Get() && m_buttonClimbLevel2.Get());
  m_InternalLevel3.SetPressed(m_buttonClimbEnable.Get() && m_buttonClimbLevel3.Get());
}