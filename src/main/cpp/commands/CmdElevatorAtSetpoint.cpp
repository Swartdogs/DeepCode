#include "commands/CmdElevatorAtSetpoint.h"
#include "Robot.h"

CmdElevatorAtSetpoint::CmdElevatorAtSetpoint() {}

void CmdElevatorAtSetpoint::Initialize() {}

void CmdElevatorAtSetpoint::Execute() {}

bool CmdElevatorAtSetpoint::IsFinished() { 
  return Robot::m_elevator.ElevatorAtSetpoint(); 
}

void CmdElevatorAtSetpoint::End() {}

void CmdElevatorAtSetpoint::Interrupted() {}
