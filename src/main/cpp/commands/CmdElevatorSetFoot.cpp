#include "Robot.h"
#include "commands/CmdElevatorSetFoot.h"

CmdElevatorSetFoot::CmdElevatorSetFoot(Elevator::FootPosition position, double timeout) {
  m_footPosition = position;
  m_timeout      = fabs(timeout);
  m_status       = csRun;
}

void CmdElevatorSetFoot::Initialize() {
  printf("Foot Position= %s %d\n", Robot::m_elevator.GetFootPositionName(Robot::m_elevator.GetFootPosition()), Robot::m_elevator.m_footSol.Get());

  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    m_status = csCancel;
    Robot::m_robotLog.Write("Elevator: Set Foot Canceled");
  } else if (m_footPosition == Robot::m_elevator.GetFootPosition()) {
    m_status = csDone;
  } else {
    Robot::m_elevator.SetFootPosition(m_footPosition);
    if (m_timeout > 0) {
      SetTimeout(m_timeout);
      m_status = csRun;
    } else {
      m_status = csDone;
    }
  }
}

void CmdElevatorSetFoot::Execute() {
  if (IsTimedOut()) m_status = csDone;
}

bool CmdElevatorSetFoot::IsFinished() {
  return (m_status != csRun);
}

void CmdElevatorSetFoot::End() {}

void CmdElevatorSetFoot::Interrupted() {}
