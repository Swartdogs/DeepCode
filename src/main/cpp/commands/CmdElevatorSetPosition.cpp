#include "Robot.h"
#include "commands/CmdElevatorSetPosition.h"

CmdElevatorSetPosition::CmdElevatorSetPosition(Elevator::ElevatorPosition position, bool waitForDone) {
  m_elevatorPosition  = position;
  m_waitForDone       = waitForDone;
  m_status            = csDone;
}

void CmdElevatorSetPosition::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    m_status = csCancel;
    Robot::m_robotLog.Write("Elevator: Set Position SKIP");
  } else {
    if (m_waitForDone) m_status = csRun;
    else               m_status = csDone;

    Robot::m_elevator.SetElevatorPosition(m_elevatorPosition);
  }
}

void CmdElevatorSetPosition::Execute() {
  if (m_status == csRun) {
    if (Robot::m_elevator.ElevatorAtSetpoint()) m_status = csDone;
  }
}

bool CmdElevatorSetPosition::IsFinished() {
  return (m_status != csRun);
}

void CmdElevatorSetPosition::End() {}

void CmdElevatorSetPosition::Interrupted() {}
