/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/CmdElevatorRaiseInches.h"
#include "Robot.h"

CmdElevatorRaiseInches::CmdElevatorRaiseInches(double inches) {
  m_inches = inches;
}

// Called just before this Command runs the first time
void CmdElevatorRaiseInches::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    // Do Nothing
  } else {
    Robot::m_elevator.SetElevatorSetpoint(
        Robot::m_elevator.GetElevatorPosition() + m_inches);
  }

  sprintf(Robot::message,
          "Elevator:    RaiseInches   Height=%5.1f",
          Robot::m_elevator.GetElevatorPosition() + m_inches);

  Robot::m_robotLog.Write(Robot::message);
}

// Called repeatedly when this Command is scheduled to run
void CmdElevatorRaiseInches::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool CmdElevatorRaiseInches::IsFinished() { return true; }

// Called once after isFinished returns true
void CmdElevatorRaiseInches::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdElevatorRaiseInches::Interrupted() {}
