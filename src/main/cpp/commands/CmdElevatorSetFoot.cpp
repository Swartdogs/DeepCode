/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"
#include "commands/CmdElevatorSetFoot.h"

CmdElevatorSetFoot::CmdElevatorSetFoot(Elevator::FootPosition position, double timeout) {
  m_footPosition = position;
  m_timeout      = fabs(timeout);
}

// Called just before this Command runs the first time
void CmdElevatorSetFoot::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    Robot::m_robotLog.Write("Elevator:    SetFoot Canceled");
  } else {
    Robot::m_elevator.SetFootPosition(m_footPosition);
    if (m_timeout > 0) SetTimeout(m_timeout);

    sprintf(Robot::message,"Elevator:    SetFoot   Position=%s", Robot::m_elevator.GetFootPositionName(m_footPosition));

    Robot::m_robotLog.Write(Robot::message);
  }
}

// Called repeatedly when this Command is scheduled to run
void CmdElevatorSetFoot::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool CmdElevatorSetFoot::IsFinished() {
  return (m_timeout = 0 || IsTimedOut());
  }

// Called once after isFinished returns true
void CmdElevatorSetFoot::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdElevatorSetFoot::Interrupted() {}
