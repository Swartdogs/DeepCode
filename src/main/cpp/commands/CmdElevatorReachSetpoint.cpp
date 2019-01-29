/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/CmdElevatorReachSetpoint.h"
#include "Robot.h"

CmdElevatorReachSetpoint::CmdElevatorReachSetpoint() {
  // Use Requires() here to declare subsystem dependencies
  // eg. Requires(Robot::chassis.get());
}

// Called just before this Command runs the first time
void CmdElevatorReachSetpoint::Initialize() {}

// Called repeatedly when this Command is scheduled to run
void CmdElevatorReachSetpoint::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool CmdElevatorReachSetpoint::IsFinished() { 
  return (Robot::m_elevator.GetElevatorSetpoint() < 0 && Robot::m_elevator.ElevatorAtSetpoint()); 
  }

// Called once after isFinished returns true
void CmdElevatorReachSetpoint::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdElevatorReachSetpoint::Interrupted() {}
