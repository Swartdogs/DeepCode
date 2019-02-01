#include "Robot.h"
#include "commands/CmdElevatorSetPosition.h"

CmdElevatorSetPosition::CmdElevatorSetPosition(Elevator::ElevatorPosition position) {
  m_elevatorPosition = position;
}

void CmdElevatorSetPosition::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    Robot::m_robotLog.Write("Elevator: Set Position Canceled");
  } else {
    Robot::m_elevator.SetElevatorPosition(m_elevatorPosition);
  }
}

void CmdElevatorSetPosition::Execute() {}

bool CmdElevatorSetPosition::IsFinished() { 
  return true; 
}

void CmdElevatorSetPosition::End() {}

void CmdElevatorSetPosition::Interrupted() {}
