#include "Robot.h"
#include "commands/CmdElevatorSetPosition.h"

CmdElevatorSetPosition::CmdElevatorSetPosition(Elevator::ElevatorPosition position, bool waitForDone, double footSpeed,
                                               double driveSpeed) {
  m_elevatorPosition  = position;
  m_waitForDone       = waitForDone;
  m_status            = csDone;
  m_footSpeed         = footSpeed;
  m_driveSpeed        = driveSpeed;
}

void CmdElevatorSetPosition::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    m_status = csCancel;
    Robot::m_robotLog.Write("Elevator: Set Position SKIP");

  } else {
    if (m_waitForDone) {
      m_status = csRun;
      Robot::m_elevator.SetFootInUse(true);
      Robot::m_drive.SetDriveInUse(true);
    } else {
      m_status = csDone;
    }

    Robot::m_elevator.SetElevatorPosition(m_elevatorPosition);
  }
}

void CmdElevatorSetPosition::Execute() {
  if (m_status == csRun) {
    if (Robot::m_elevator.ElevatorAtSetpoint()) m_status = csDone;

    Robot::m_elevator.DriveFoot(m_footSpeed);
    Robot::m_drive.ArcadeDrive(m_driveSpeed, 0);
  }
}

bool CmdElevatorSetPosition::IsFinished() {
  return (m_status != csRun);
}

void CmdElevatorSetPosition::End() {
 if (m_waitForDone) {
    Robot::m_elevator.SetFootInUse(false);
    Robot::m_drive.SetDriveInUse(false);
 }
}

void CmdElevatorSetPosition::Interrupted() {
  End();
}
