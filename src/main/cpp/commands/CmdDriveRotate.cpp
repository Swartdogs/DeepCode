
#include <commands/CmdDriveRotate.h>
#include "Robot.h"

CmdDriveRotate::CmdDriveRotate(double heading, double maxSpeed, bool resetGyro, double timeout, bool useSonar) {
  Requires(&Robot::m_drive); 

  m_heading      = heading;
  m_maxSpeed     = maxSpeed;
  m_resetGyro    = resetGyro;
  m_timeout      = fabs(timeout); 
  m_useSonar     = useSonar;
  m_status       = csRun; 
}

void CmdDriveRotate::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    m_status = csSkip;
  } else {
    m_status = csRun;
    Robot::m_drive.SetDriveInUse(true);
    Robot::m_drive.SetBrakeMode(false);

    if (m_useSonar) m_heading = Robot::m_drive.GetSonarAngle();

    Robot::m_drive.RotateInit(m_heading, m_maxSpeed, m_resetGyro);
    
    if (m_timeout > 0) SetTimeout(m_timeout);

    sprintf(Robot::message, "Drive:  Rotate INIT   Heading=%5.1f to %5.1f  MaxSpeed=%3.1f", 
            Robot::m_drive.GetHeading(), m_heading, m_maxSpeed);

    Robot::m_robotLog.Write(Robot::message);
  }
}

void CmdDriveRotate::Execute() {
  double rotate = 0;

  if(m_status == csRun) {
    if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
      m_status = csCancel;
    } else if (IsTimedOut()) {
      m_status = csTimedOut;
      if (this->IsParented()) this->GetGroup()->Cancel();
    } else if (Robot::m_drive.RotateIsFinished()) {
      m_status = csDone;
    } else {
      rotate = Robot::m_drive.RotateExec();
    }
  }

  Robot::m_drive.ArcadeDrive(0, rotate);
}

bool CmdDriveRotate::IsFinished() { 
  return (m_status != csRun); 
}

void CmdDriveRotate::End() {
  Robot::m_drive.SetDriveInUse(false);

  switch (m_status) {
    case csSkip:
      sprintf(Robot::message, "Drive:   Rotate SKIP");
      break;

    case csDone:
      sprintf(Robot::message, "Drive:   Rotate DONE       Heading=%5.1f", Robot::m_drive.GetHeading());
      break;

    case csCancel:
      sprintf(Robot::message, "Drive:   Rotate CANCELED   Heading=%5.1f", Robot::m_drive.GetHeading());
      break;

    case csTimedOut:
      sprintf(Robot::message, "Drive:   Rotate TIMED OUT  Heading=%5.1f", Robot::m_drive.GetHeading());
      break;
      
    default:;
  }

  Robot::m_robotLog.Write(Robot::message);
}

void CmdDriveRotate::Interrupted() {
  m_status = csCancel;
  End();
}
