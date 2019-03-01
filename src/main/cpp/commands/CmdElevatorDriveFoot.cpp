// Drive the Elevator Foot at a specified speed until the specified Floor sensor
//   detects floor or a timeout occurs.

#include "commands/CmdElevatorDriveFoot.h"
#include "Robot.h"

CmdElevatorDriveFoot::CmdElevatorDriveFoot(Elevator::FloorSensor floorSensor, double footSpeed, double driveSpeed,
                                           double timeout, int addTime) {
  Requires(&Robot::m_drive);

  m_driveSpeed      = driveSpeed;
  m_footSpeed       = footSpeed;
  m_floorSensor     = floorSensor;
  m_status          = csRun;
  m_timeout         = fabs(timeout);  
  m_stopCounter     = 0;
  m_addTime         = addTime;
}

void CmdElevatorDriveFoot::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false){           // Skip command if in a Group that has been canceled
      m_status = csSkip;
  } else {
    m_status = csRun;
    m_stopCounter = 0;
    Robot::m_elevator.SetFootInUse(true);                                       // Set Foot-im-use flag
    Robot::m_drive.SetDriveInUse(true);                                         // Set Drive-in-use flag
    Robot::m_drive.SetBrakeMode(true);                                          // Set motor controllers to Brake mode

    if (m_timeout > 0) SetTimeout(m_timeout);                                   // Set timeout

    sprintf(Robot::message, "Elevator: Drive Foot at %3.1f for %4.1f seconds", m_footSpeed, m_timeout);
    Robot::m_robotLog.Write(Robot::message);
  }
}

void CmdElevatorDriveFoot::Execute() {
  double driveSpeed = 0;
  double footSpeed  = 0;

  if(m_status == csRun){
      if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false){       // Cancel if in Group that has been canceled
        m_status = csCancel;
      } else if (IsTimedOut()) {                                                // End command and cancel Group if TimedOut
        m_status = csTimedOut;
        if (this->IsParented()) this->GetGroup()->Cancel();
      } else if (Robot::m_elevator.FloorDetected(m_floorSensor)) {              // Increment stop counter if floor detected
        m_stopCounter++;
        if (m_stopCounter >= m_addTime) m_status = csDone;                      // Set to Done when stop counter reaches target
      } else {
        driveSpeed = m_driveSpeed;                                              // Set drive and foot speed to specified valeus
        footSpeed = m_footSpeed;
      }
  }

  Robot::m_elevator.DriveFoot(footSpeed);
  Robot::m_drive.ArcadeDrive(driveSpeed, 0); 
}

bool CmdElevatorDriveFoot::IsFinished() { 
  return (m_status != csRun);                                                   // Finished when status is not in Run state
}

void CmdElevatorDriveFoot::End() {
  Robot::m_elevator.SetFootInUse(false);                                        // Clear Foot-in-use flag
  Robot::m_drive.SetDriveInUse(false);                                          // Clear Drive-in-use flag

  switch (m_status) {
    case csSkip:
      sprintf(Robot::message, "Elevator: Drive Foot SKIP");
      break;

    case csDone:
      sprintf(Robot::message, "Elevator: Drive Foot DONE");
      break;

    case csCancel:
      sprintf(Robot::message, "Elevator: Drive Foot CANCELED");
      break;

    case csTimedOut:
      sprintf(Robot::message, "Elevator: Drive Foot TIMED OUT");
      break;
      
    default:;
  }

  Robot::m_robotLog.Write(Robot::message);
}

void CmdElevatorDriveFoot::Interrupted() {
  m_status = csCancel;
  End();
}
