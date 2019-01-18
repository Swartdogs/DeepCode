
#include <commands/CmdDriveRotate.h>
#include <Robot.h>



CmdDriveRotate::CmdDriveRotate(double heading, double maxSpeed, bool resetGyro, double timeout) {
  Requires(&Robot::m_drive); 

  m_heading      = heading;
  m_maxSpeed     = maxSpeed;
  m_resetGyro    = resetGyro;
  m_timeout      = fabs(timeout); 
  m_status       = csRun;
  
}

// Called just before this Command runs the first time
void CmdDriveRotate::Initialize() {
   if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
      m_status = csSkip;
   } else {
     m_status = csRun;

     Robot::m_drive.RotateInit(m_heading, m_maxSpeed, m_resetGyro);
     if (m_timeout > 0) SetTimeout(m_timeout);

     sprintf(Robot::message, "Drive:  Rotate INIT   Heading=%5.1f to %5.1f  MaxSpeed=%3.1f", 
             Robot::m_drive.GetHeading(), m_heading, m_maxSpeed);

      //Robot::m_log.Write(Robot::message);
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


void CmdDriveRotate::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdDriveRotate::Interrupted() {}
