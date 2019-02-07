#include "commands/CmdDriveJoystick.h"
#include "Robot.h"
#include "subsystems/Drive.h"
#include "OI.h"

CmdDriveJoystick::CmdDriveJoystick() {
  Requires(&Robot::m_drive);
}

void CmdDriveJoystick::Initialize() {
  Robot::m_drive.SetDriveInUse(true);
}

void CmdDriveJoystick::Execute() {
  Robot::m_drive.ArcadeDrive( Robot::m_oi.GetDriveJoystickY(), 
                              Robot::m_oi.GetDriveJoystickX());
}

bool CmdDriveJoystick::IsFinished() { return false; }

void CmdDriveJoystick::End() {}

void CmdDriveJoystick::Interrupted() {
  Robot::m_drive.SetDriveInUse(false);
}
