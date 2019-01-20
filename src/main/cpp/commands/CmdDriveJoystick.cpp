/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/CmdDriveJoystick.h"
#include "Robot.h"
#include "subsystems/Drive.h"
#include "OI.h"

CmdDriveJoystick::CmdDriveJoystick() {
  Requires(&Robot::m_drive);
}

// Called just before this Command runs the first time
void CmdDriveJoystick::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void CmdDriveJoystick::Execute() {
  Robot::m_drive.ArcadeDrive( Robot::m_oi.ApplyDeadband(Robot::m_oi.GetDriveJoystickY(), 0.1), 
                              Robot::m_oi.ApplyDeadband(Robot::m_oi.GetDriveJoystickX(), 0.1));
}

// Make this return true when this Command no longer needs to run execute()
bool CmdDriveJoystick::IsFinished() { return false; }

// Called once after isFinished returns true
void CmdDriveJoystick::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdDriveJoystick::Interrupted() {}
