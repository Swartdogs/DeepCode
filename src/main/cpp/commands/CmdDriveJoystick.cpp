// Drive Robot using the Joystick  (Default Drive subsystem command)

#include "commands/CmdDriveJoystick.h"
#include "Robot.h"
#include "subsystems/Drive.h"
#include "OI.h"

CmdDriveJoystick::CmdDriveJoystick() {
  Requires(&Robot::m_drive);
  m_brakesOn = true;
}

void CmdDriveJoystick::Initialize() {
  Robot::m_drive.SetDriveInUse(true);                                           // Set Drive-in-Use flag
  m_brakesOn = true;
}

void CmdDriveJoystick::Execute() {                                              // Drive using joystick values
  double drive  = Robot::m_oi.GetDriveJoystickY();
  double rotate = Robot::m_oi.GetDriveJoystickX();

  if (m_brakesOn) {
    if ((fabs(drive) > 0.1) || fabs(rotate) > 0.1) {
      m_brakesOn = false;
      Robot::m_drive.SetBrakeMode(false);                                       // Set speed controllers to Coast Mode
    }
  }

  Robot::m_drive.ArcadeDrive(drive, rotate);
}

bool CmdDriveJoystick::IsFinished() { return false; }

void CmdDriveJoystick::End() {}

void CmdDriveJoystick::Interrupted() {
  Robot::m_drive.SetDriveInUse(false);
}
