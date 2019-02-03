#include "OI.h"
#include <frc/WPILib.h>
#include "commands/GrpClimb.h"
#include "commands/CmdCancelClimb.h"
#include "subsystems/Elevator.h"

OI::OI() {
  m_InternalLevel2.WhenPressed    (new GrpClimb(Elevator::epLevel2));
  m_InternalLevel3.WhenPressed    (new GrpClimb(Elevator::epLevel3));
  m_buttonClimbCancel.WhenPressed (new CmdCancelClimb());
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

double OI::GetArmJoystickX() {
  return m_armJoystick.GetX();
}

double OI::GetArmJoystickY() {
  return -m_armJoystick.GetY();
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