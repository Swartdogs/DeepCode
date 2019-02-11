#include "commands/CmdElevatorSetHeight.h"
#include "Robot.h"

CmdElevatorSetHeight::CmdElevatorSetHeight(double inches) {
  m_inches = inches;
}

void CmdElevatorSetHeight::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    Robot::m_robotLog.Write("Elevator: Set Height SKIP");
  } else {
    Robot::m_elevator.SetElevatorSetpoint( m_inches );
  }
}

void CmdElevatorSetHeight::Execute() {}

bool CmdElevatorSetHeight::IsFinished() { return true; }

void CmdElevatorSetHeight::End() {}

void CmdElevatorSetHeight::Interrupted() {}
