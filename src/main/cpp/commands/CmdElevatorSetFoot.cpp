// Set the Elevator to Extended or Retracted
//   and optionally wait a specified period of time for action to complete

#include "Robot.h"
#include "commands/CmdElevatorSetFoot.h"

CmdElevatorSetFoot::CmdElevatorSetFoot(Elevator::FootPosition position, double timeout) {
  m_footPosition = position;
  m_timeout      = fabs(timeout);
  m_status       = csRun;
}

void CmdElevatorSetFoot::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {          // Skip command if in a Group that has been canceled
    m_status = csCancel;
    Robot::m_robotLog.Write("Elevator: Set Foot Canceled");
  } else if (m_footPosition == Robot::m_elevator.GetFootPosition()) {           // Set to done if Foot already in specified position
    m_status = csDone;
  } else {                                                                      // Set Foot to new position
    Robot::m_elevator.SetFootPosition(m_footPosition);
    if (m_timeout > 0) {                                                        // Set optional timeout for action to complete
      SetTimeout(m_timeout);
      m_status = csRun;
    } else {                                                                    // No wait for completion
      m_status = csDone;
    }
  }
}

void CmdElevatorSetFoot::Execute() {
  if (IsTimedOut()) m_status = csDone;                                          // Set to Done when TimedOut
}

bool CmdElevatorSetFoot::IsFinished() {
  return (m_status != csRun);                                                   // Finished when status is not in Run state
}

void CmdElevatorSetFoot::End() {}

void CmdElevatorSetFoot::Interrupted() {}
