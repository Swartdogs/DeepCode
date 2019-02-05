/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/CmdArmSetWristPosition.h"
#include "Robot.h"

CmdArmSetWristPosition::CmdArmSetWristPosition(Arm::ArmPosition position) {
  m_position = position;
}

// Called just before this Command runs the first time
void CmdArmSetWristPosition::Initialize() {
  Robot::m_arm.SetWristPosition(m_position);
}

// Called repeatedly when this Command is scheduled to run
void CmdArmSetWristPosition::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool CmdArmSetWristPosition::IsFinished() { return true; }

// Called once after isFinished returns true
void CmdArmSetWristPosition::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdArmSetWristPosition::Interrupted() {}
