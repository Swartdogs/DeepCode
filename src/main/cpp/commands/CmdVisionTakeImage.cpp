/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"
#include "commands/CmdVisionTakeImage.h"

CmdVisionTakeImage::CmdVisionTakeImage(Vision::TargetSelect targetSelect) {
  Requires(&Robot::m_vision);
}

// Called just before this Command runs the first time
void CmdVisionTakeImage::Initialize() {
  Robot::m_vision.FindTarget(m_targetSelect);
}

// Called repeatedly when this Command is scheduled to run
void CmdVisionTakeImage::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool CmdVisionTakeImage::IsFinished() {
  return (Robot::m_vision.GetSearchState() != Vision::ssLooking); 
  }

// Called once after isFinished returns true
void CmdVisionTakeImage::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdVisionTakeImage::Interrupted() {}
