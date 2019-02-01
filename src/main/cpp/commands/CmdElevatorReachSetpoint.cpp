#include "commands/CmdElevatorReachSetpoint.h"
#include "Robot.h"

CmdElevatorReachSetpoint::CmdElevatorReachSetpoint() {

}

void CmdElevatorReachSetpoint::Initialize() {}

void CmdElevatorReachSetpoint::Execute() {}

bool CmdElevatorReachSetpoint::IsFinished() { 
  return (Robot::m_elevator.GetElevatorSetpoint() < 0 && Robot::m_elevator.ElevatorAtSetpoint()); 
  }

void CmdElevatorReachSetpoint::End() {}

void CmdElevatorReachSetpoint::Interrupted() {}
