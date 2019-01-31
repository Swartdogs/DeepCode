/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"
#include "commands/CmdElevatorSetPosition.h"

CmdElevatorSetPosition::CmdElevatorSetPosition(Elevator::ElevatorPosition position) {
  m_elevatorPosition = position;
}

// Called just before this Command runs the first time
void CmdElevatorSetPosition::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    Robot::m_robotLog.Write("Elevator:    SetPosition Canceled");
  } else {
    Robot::m_elevator.SetElevatorPosition(m_elevatorPosition);
  }
}

// Called repeatedly when this Command is scheduled to run
void CmdElevatorSetPosition::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool CmdElevatorSetPosition::IsFinished() { return true; }

// Called once after isFinished returns true
void CmdElevatorSetPosition::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdElevatorSetPosition::Interrupted() {}
