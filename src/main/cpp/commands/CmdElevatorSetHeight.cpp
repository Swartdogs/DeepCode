/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/CmdElevatorSetHeight.h"

CmdElevatorSetHeight::CmdElevatorSetHeight(double inches) {
  m_inches = inches;
}

// Called just before this Command runs the first time
void CmdElevatorSetHeight::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    //Do Nothing
  } else {
    Robot::m_elevator.SetElevatorSetpoint( m_inches * Elevator::COUNTS_PER_INCH);
  }
}


// Called repeatedly when this Command is scheduled to run
void CmdElevatorSetHeight::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool CmdElevatorSetHeight::IsFinished() { return true; }

// Called once after isFinished returns true
void CmdElevatorSetHeight::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdElevatorSetHeight::Interrupted() {}