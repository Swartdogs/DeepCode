// Set Elevator to a specified position and optionally wait until completed

#include "Robot.h"
#include "commands/CmdElevatorSetPosition.h"

CmdElevatorSetPosition::CmdElevatorSetPosition(Elevator::ElevatorPosition position, bool waitForDone, double timeout, double footSpeed,
                                               double driveSpeed) {
  m_elevatorPosition  = position;
  m_waitForDone       = waitForDone;
  m_status            = csDone;
  m_footSpeed         = footSpeed;
  m_driveSpeed        = driveSpeed;
  m_timeout           = timeout;
}

void CmdElevatorSetPosition::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {          // Skip command if in a Group that has been canceled
    m_status = csCancel;
    Robot::m_robotLog.Write("Elevator: Set Position SKIP");

  } else {
    if (m_waitForDone) {                                                        // Optional Wait for completion
      m_status = csRun;

      if (m_timeout > 0) SetTimeout(m_timeout);

      Robot::m_elevator.SetFootInUse(true);                                     // Set Foot-in-use flag
      Robot::m_drive.SetDriveInUse(true);                                       // Set Drive-in-use flag
    } else {
      m_status = csDone;                                                        // No wait for completion
    }

    Robot::m_elevator.SetElevatorPosition(m_elevatorPosition);                  // Set new Elevator position
  }
}

void CmdElevatorSetPosition::Execute() {
  if (m_status == csRun) {
    if (Robot::m_elevator.ElevatorAtSetpoint()) {
      m_status = csDone;
    } else if(IsTimedOut()) {
      m_status = csTimedOut;
      if (this->IsParented()) this->GetGroup()->Cancel();
    }                                                                           // Set to Done when Elevator is at Setpoint

    Robot::m_elevator.DriveFoot(m_footSpeed);                                   // Drive Foot at specified speed
    Robot::m_drive.ArcadeDrive(m_driveSpeed, 0);                                // Drive Robot at specified speed
  }
}

bool CmdElevatorSetPosition::IsFinished() {
  return (m_status != csRun);                                                   // Finished when status is not in Run state
}

void CmdElevatorSetPosition::End() {
 if (m_waitForDone) {
    Robot::m_elevator.SetFootInUse(false);                                      // Clear Foot-in-use flag
    Robot::m_drive.SetDriveInUse(false);                                        // Clear Drive-in-use flag

    if (m_status == csTimedOut) Robot::m_robotLog.Write("Elevator: Set Position TIMED OUT");
  }
}

void CmdElevatorSetPosition::Interrupted() {
  End();
}
