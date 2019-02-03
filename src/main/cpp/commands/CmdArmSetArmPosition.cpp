/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/CmdArmSetArmPosition.h"
#include "Robot.h"

CmdArmSetArmPosition::CmdArmSetArmPosition(Arm::ArmPosition position) {
  m_position = position; 
}

// Called just before this Command runs the first time
void CmdArmSetArmPosition::Initialize() {
  Robot::m_arm.SetShoulderPosition(m_position);
  Robot::m_arm.SetWristPosition(m_position);
}

// Called repeatedly when this Command is scheduled to run
void CmdArmSetArmPosition::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool CmdArmSetArmPosition::IsFinished() { return true; }

// Called once after isFinished returns true
void CmdArmSetArmPosition::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdArmSetArmPosition::Interrupted() {}
