/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/CmdArmSetManual.h"
#include "Robot.h"

CmdArmSetManual::CmdArmSetManual(bool isManual) {
  m_isManual = isManual;
}

// Called just before this Command runs the first time
void CmdArmSetManual::Initialize() {
  Robot::m_arm.SetDrivenManually(m_isManual);
}

// Called repeatedly when this Command is scheduled to run
void CmdArmSetManual::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool CmdArmSetManual::IsFinished() { return true; }

// Called once after isFinished returns true
void CmdArmSetManual::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdArmSetManual::Interrupted() {}
