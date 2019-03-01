// Drive Robot using the Joystick  (Default Drive subsystem command)

#include "commands/CmdDriveJoystick.h"
#include "Robot.h"
#include "subsystems/Drive.h"
#include "OI.h"

CmdDriveJoystick::CmdDriveJoystick() {
  Requires(&Robot::m_drive);
}

void CmdDriveJoystick::Initialize() {
  Robot::m_drive.SetDriveInUse(true);                                           // Set Drive-in-Use flag
  Robot::m_drive.SetBrakeMode(false);                                           // Set speed controllers to Coast Mode
}

void CmdDriveJoystick::Execute() {                                              // Drive using joystick values
  Robot::m_drive.ArcadeDrive( Robot::m_oi.GetDriveJoystickY(), 
                               Robot::m_oi.GetDriveJoystickX());
}

bool CmdDriveJoystick::IsFinished() { return false; }

void CmdDriveJoystick::End() {}

void CmdDriveJoystick::Interrupted() {
  Robot::m_drive.SetDriveInUse(false);
}
