#include "commands/CmdDriveDistance.h"
#include "Robot.h"
#include "subsystems/Drive.h"

CmdDriveDistance::CmdDriveDistance(double distance, double heading, double maxSpeed, double minSpeed, bool resetEncoders, 
                                   bool resetGyro, double timeout, bool useSonar, bool groupContinue) {
  Requires(&Robot::m_drive);

  m_distance       = distance;
  m_heading        = heading;
  m_maxSpeed       = maxSpeed;
  m_minSpeed       = minSpeed;
  m_resetEncoders  = resetEncoders;
  m_resetGyro      = resetGyro;
  m_timeout        = fabs(timeout);
  m_useSonar       = useSonar;
  m_groupContinue  = groupContinue;
  m_status         = csRun;
}

void CmdDriveDistance::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    m_status = csSkip;
  } else {
    m_status = csRun;
    Robot::m_drive.SetDriveInUse(true);
    Robot::m_drive.SetBrakeMode(true);

    if (m_useSonar) m_distance = Robot::m_drive.GetSonarDistance() - fabs(m_distance);

    Robot::m_drive.DriveInit(m_distance, m_heading, m_maxSpeed, m_minSpeed, m_resetEncoders, m_resetGyro, false);

    if (m_timeout > 0) SetTimeout(m_timeout);
    
    sprintf(Robot::message, "Drive:    Distance INIT  Distance=%5.1f to %5.1f Heading=%5.1f to %5.1f  MaxSpeed=%3.1f", 
            Robot::m_drive.GetDistance(Drive::ueCurrentEncoder), m_distance, Robot::m_drive.GetHeading(), m_heading, m_maxSpeed);

    Robot::m_robotLog.Write(Robot::message);
  }
}

void CmdDriveDistance::Execute() {
  double drive = 0;
  double rotate = 0;

  if(m_status == csRun) {
    if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
      m_status = csCancel;
    } else if (IsTimedOut()) {
      m_status = csTimedOut;
      if (this->IsParented()) this->GetGroup()->Cancel();
    } else if (Robot::m_drive.DriveIsFinished()) {
      m_status = csDone;
    } else {
       drive  = Robot::m_drive.DriveExec();
       rotate = Robot::m_drive.RotateExec();
    }
  }

  Robot::m_drive.ArcadeDrive(drive, rotate);
}

bool CmdDriveDistance::IsFinished() { 
    return (m_status != csRun); 
}

void CmdDriveDistance::End() {
  Robot::m_drive.SetDriveInUse(false);

  switch (m_status) {
    case csSkip:
      sprintf(Robot::message, "Drive:    Distance SKIP");
      break;

    case csDone:
      sprintf(Robot::message, "Drive:    Distance DONE   Heading=%5.1f", Robot::m_drive.GetHeading());
      break;

    case csCancel:
      sprintf(Robot::message, "Drive:    Distance CANCELED   Heading=%5.1f", Robot::m_drive.GetHeading());
      break;

    case csTimedOut:
      sprintf(Robot::message, "Drive:    Distance TIMED OUT   Heading=%5.1f", Robot::m_drive.GetHeading());
      break;
      
    default:;
  }

  Robot::m_robotLog.Write(Robot::message);
}

void CmdDriveDistance::Interrupted() {
  m_status = csCancel;
  End();
}
