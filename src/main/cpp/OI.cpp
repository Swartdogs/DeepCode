#include "OI.h"
#include <frc/WPILib.h>
#include "commands/GrpClimb.h"
#include "commands/CmdCancelClimb.h"
#include "commands/CmdDriveSetGear.h"
#include "subsystems/Elevator.h"
#include "subsystems/Drive.h"

OI::OI() {
  //m_InternalLevel2.WhenPressed    (new GrpClimb(Elevator::epLevel2));
  //m_InternalLevel3.WhenPressed    (new GrpClimb(Elevator::epLevel3));
  m_buttonClimbEnable.WhenPressed (new GrpClimb(Elevator::epLevel2));
  m_buttonClimbCancel.WhenPressed (new CmdCancelClimb());
  m_buttonShifter.WhenPressed     (new CmdDriveSetGear(Drive::spLow));
  m_buttonShifter.WhenReleased    (new CmdDriveSetGear(Drive::spHigh));
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
  //return ApplyDeadband(m_armJoystick.GetX(), 0.05);
  return 0;
}

double OI::GetArmJoystickY() {
  //return -ApplyDeadband(m_armJoystick.GetY(), 0.05);
  return 0;
}

double OI::GetDriveJoystickX(){
  return ApplyDeadband(m_driveJoystick.GetX(), 0.05);
}

double OI::GetDriveJoystickY(){
  return -ApplyDeadband(m_driveJoystick.GetY(), 0.05);
}

void OI::Periodic() {
  //m_InternalLevel2.SetPressed(m_buttonClimbEnable.Get() && m_buttonClimbLevel2.Get());
  //m_InternalLevel3.SetPressed(m_buttonClimbEnable.Get() && m_buttonClimbLevel3.Get());
}