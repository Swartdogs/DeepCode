#include "commands/CmdDriveFoot.h"
#include "Robot.h"

CmdDriveFoot::CmdDriveFoot(Elevator::FloorSensor floorSensor, double footSpeed, double timeout) {
  Requires(&Robot::m_drive);

  m_footSpeed       = footSpeed;
  m_floorSensor     = floorSensor;
  m_status          = csRun;
  m_timeout         = fabs(timeout);
}

void CmdDriveFoot::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false){
      m_status = csSkip;
  } else {
    m_status = csRun;
    Robot::m_elevator.SetFootInUse(true);
    Robot::m_drive.SetDriveInUse(true);
    if (m_timeout > 0) SetTimeout(m_timeout);

    sprintf(Robot::message, "Elevator: Drive Foot at %3.1f for %4.1f seconds", m_footSpeed, m_timeout);
    Robot::m_robotLog.Write(Robot::message);
  }
}

void CmdDriveFoot::Execute() {
  double speed = 0;

  if(m_status == csRun){
      if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false){
        m_status = csCancel;
      } else if (IsTimedOut()) {
        m_status = csTimedOut;
        if (this->IsParented()) this->GetGroup()->Cancel();
      } else if (Robot::m_elevator.FloorDetected(m_floorSensor)) {
        m_status = csDone;
      } else {
        speed = m_footSpeed;
      }
  }

  Robot::m_elevator.DriveFoot(speed);
  Robot::m_drive.ArcadeDrive(speed * Robot::m_dashboard.GetDashValue(dvDriveRatio), 0); 
}

bool CmdDriveFoot::IsFinished() { 
  return (m_status != csRun);
}

void CmdDriveFoot::End() {
  Robot::m_elevator.SetFootInUse(false);
  Robot::m_drive.SetDriveInUse(false);

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

void CmdDriveFoot::Interrupted() {
  m_status = csCancel;
  End();
}
