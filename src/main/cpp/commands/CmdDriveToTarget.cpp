// Drive Robot using PID control to Target determined by the Vision subsystem
//   and wait for completion or timeout

#include "commands/CmdDriveToTarget.h"
#include "Robot.h"

CmdDriveToTarget::CmdDriveToTarget(double addToDistance, double maxSpeed, double timeout) {
  Requires(&Robot::m_drive);

  m_addToDistance = addToDistance;
  m_maxSpeed      = maxSpeed;
  m_distance      = 0;
  m_heading       = 0;
  m_timeout       = timeout;
  m_status        = csRun;
}

void CmdDriveToTarget::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    m_status = csSkip;
  } else {
    m_status = csRun;
    Robot::m_drive.SetDriveInUse(true);
    Robot::m_drive.SetBrakeMode(true);

    m_distance  = Robot::m_vision.GetTargetDistance() + m_addToDistance;
    m_heading   = Robot::m_vision.GetTargetAngle() + Robot::m_dashboard.GetDashValue(dvVisionAngleOffset);
    
    Robot::m_drive.DriveInit(m_distance, m_heading, m_maxSpeed, 0, true, true);

    if (m_timeout > 0) SetTimeout(m_timeout);
    
    sprintf(Robot::message, "Drive:    To Target INIT  Distance=%5.1f Heading=%5.1f", m_distance, m_heading);
    Robot::m_robotLog.Write(Robot::message);
  }
}

void CmdDriveToTarget::Execute() {
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

bool CmdDriveToTarget::IsFinished() { 
  return (m_status != csRun); 
}

void CmdDriveToTarget::End() {
  Robot::m_drive.SetDriveInUse(false);

  switch (m_status) {
    case csSkip:
      sprintf(Robot::message, "Drive:    To Target SKIP");
      break;

    case csDone:
      sprintf(Robot::message, "Drive:    To Target DONE  Distance Left=%5.1f  Right = %5.1f  Heading=%5.1f", 
              Robot::m_drive.GetDistance(Drive::ueLeftEncoder), Robot::m_drive.GetDistance(Drive::ueRightEncoder), 
              Robot::m_drive.GetHeading());
      break;

    case csCancel:
      sprintf(Robot::message, "Drive:    To Target CANCELED  Distance Left=%5.1f  Right = %5.1f  Heading=%5.1f", 
              Robot::m_drive.GetDistance(Drive::ueLeftEncoder), Robot::m_drive.GetDistance(Drive::ueRightEncoder), 
              Robot::m_drive.GetHeading());
      break;

    case csTimedOut:
      sprintf(Robot::message, "Drive:    To Target TIMED OUT  Distance Left=%5.1f  Right = %5.1f  Heading=%5.1f", 
              Robot::m_drive.GetDistance(Drive::ueLeftEncoder), Robot::m_drive.GetDistance(Drive::ueRightEncoder), 
              Robot::m_drive.GetHeading());
      break;
      
    default:;
  }

  Robot::m_robotLog.Write(Robot::message);
}

void CmdDriveToTarget::Interrupted() {
  m_status = csCancel;
  End();
}
