/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/CmdDriveFoot.h"
#include "Robot.h"

CmdDriveFoot::CmdDriveFoot(Elevator::PlatformStatus platformStatus, double footSpeed) {
  m_footSpeed       = footSpeed;
  m_platformStatus  = platformStatus;
  m_status          = csRun;
}

// Called just before this Command runs the first time
void CmdDriveFoot::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false){
      m_status = csSkip;
  } 
}

// Called repeatedly when this Command is scheduled to run
void CmdDriveFoot::Execute() {
  if(m_status == csRun){
    Robot::m_elevator.DriveFoot(m_footSpeed);
  }
}

// Make this return true when this Command no longer needs to run execute()
bool CmdDriveFoot::IsFinished() { return false; }

// Called once after isFinished returns true
void CmdDriveFoot::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdDriveFoot::Interrupted() {}
