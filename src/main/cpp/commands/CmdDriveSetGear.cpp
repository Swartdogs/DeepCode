/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/CmdDriveSetGear.h"
#include "Robot.h"

CmdDriveSetGear::CmdDriveSetGear(Drive::ShifterPosition position) {
  m_position = position;
}

// Called just before this Command runs the first time
void CmdDriveSetGear::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    //Do nothing
  } else {
    Robot::m_drive.SetShifter(m_position);
  }
}

// Called repeatedly when this Command is scheduled to run
void CmdDriveSetGear::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool CmdDriveSetGear::IsFinished() { return true; }

// Called once after isFinished returns true
void CmdDriveSetGear::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdDriveSetGear::Interrupted() {}
