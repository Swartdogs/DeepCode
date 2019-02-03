/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/CmdArmSetWristDegrees.h"
#include "Robot.h"

CmdArmSetWristDegrees::CmdArmSetWristDegrees(double degrees) {
  m_degrees = degrees;
}

// Called just before this Command runs the first time
void CmdArmSetWristDegrees::Initialize() {
  Robot::m_arm.SetWristPosition(m_degrees);
}

// Called repeatedly when this Command is scheduled to run
void CmdArmSetWristDegrees::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool CmdArmSetWristDegrees::IsFinished() { return true; }

// Called once after isFinished returns true
void CmdArmSetWristDegrees::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdArmSetWristDegrees::Interrupted() {}
