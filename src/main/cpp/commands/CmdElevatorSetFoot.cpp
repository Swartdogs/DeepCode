#include "Robot.h"
#include "commands/CmdElevatorSetFoot.h"

CmdElevatorSetFoot::CmdElevatorSetFoot(Elevator::FootPosition position, double timeout) {
  m_footPosition = position;
  m_timeout      = fabs(timeout);
}

void CmdElevatorSetFoot::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    Robot::m_robotLog.Write("Elevator: Set Foot Canceled");
  } else {
    Robot::m_elevator.SetFootPosition(m_footPosition);
    if (m_timeout > 0) SetTimeout(m_timeout);
  }
}

void CmdElevatorSetFoot::Execute() {}

bool CmdElevatorSetFoot::IsFinished() {
  return (m_timeout = 0 || IsTimedOut());
  }

void CmdElevatorSetFoot::End() {}

void CmdElevatorSetFoot::Interrupted() {}
