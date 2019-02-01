#include "commands/CmdElevatorRaiseInches.h"
#include "Robot.h"

CmdElevatorRaiseInches::CmdElevatorRaiseInches(double inches) {
  m_inches = inches;
}

void CmdElevatorRaiseInches::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    Robot::m_robotLog.Write("Elevator: Change Position Canceled");
  } else {
    Robot::m_elevator.SetElevatorSetpoint(Robot::m_elevator.GetElevatorPosition() + m_inches);
  }
}

void CmdElevatorRaiseInches::Execute() {}

bool CmdElevatorRaiseInches::IsFinished() { 
  return true; 
}

void CmdElevatorRaiseInches::End() {}

void CmdElevatorRaiseInches::Interrupted() {}
