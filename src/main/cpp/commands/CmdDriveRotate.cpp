
// Rotate the Robot using PID control to a specified Heading
//   and wait for completion or timeout

#include <commands/CmdDriveRotate.h>
#include "Robot.h"

CmdDriveRotate::CmdDriveRotate(double heading, double maxSpeed, bool resetGyro, double timeout) {
  Requires(&Robot::m_drive); 

  m_heading      = heading;
  m_maxSpeed     = maxSpeed;
  m_resetGyro    = resetGyro;
  m_timeout      = fabs(timeout); 
  m_status       = csRun; 
}

void CmdDriveRotate::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {          // Skip command if in a Group that has been canceled
    m_status = csSkip;
    
  } else {
    m_status = csRun;
    Robot::m_drive.SetDriveInUse(true);                                         // Set Drive-in-use flag
    Robot::m_drive.SetBrakeMode(false);                                         // Set motor controllers to Brake mode

    Robot::m_drive.RotateInit(m_heading, m_maxSpeed, m_resetGyro);              // Initialize rotate PID
    
    if (m_timeout > 0) SetTimeout(m_timeout);                                   // Set timeout

    sprintf(Robot::message, "Drive:    Rotate INIT   Heading=%5.1f to %5.1f  MaxSpeed=%3.1f", 
            Robot::m_drive.GetHeading(), m_heading, m_maxSpeed);

    Robot::m_robotLog.Write(Robot::message);
  }
}

void CmdDriveRotate::Execute() {
  double rotate = 0;

  if(m_status == csRun) {
    if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {        // Cancel if in Group that has been canceled
      m_status = csCancel;
    
    } else if (IsTimedOut()) {                                                  // End command and cancel Group if TimedOut
      m_status = csTimedOut;
      if (this->IsParented()) this->GetGroup()->Cancel();

    } else if (Robot::m_oi.DriverCancel()) {                                    // Canceled by Driver with Drive Joystick
      m_status = csCancel;
      if (this->IsParented()) this->GetGroup()->Cancel();

    } else if (Robot::m_drive.RotateIsFinished()) {                             // Done if angle has been reached
      m_status = csDone;
    
    } else {
      rotate = Robot::m_drive.RotateExec();                                     // Get rotate value from PID
    }
  }

  Robot::m_drive.ArcadeDrive(0, rotate);
}

bool CmdDriveRotate::IsFinished() { 
  return (m_status != csRun);                                                   // Finished when status is not in Run state
}

void CmdDriveRotate::End() {
  Robot::m_drive.SetDriveInUse(false);

  switch (m_status) {
    case csSkip:
      sprintf(Robot::message, "Drive:    Rotate SKIP");
      break;

    case csDone:
      sprintf(Robot::message, "Drive:    Rotate DONE  Heading=%5.1f", Robot::m_drive.GetHeading());
      break;

    case csCancel:
      sprintf(Robot::message, "Drive:    Rotate CANCELED  Heading=%5.1f", Robot::m_drive.GetHeading());
      break;

    case csTimedOut:
      sprintf(Robot::message, "Drive:    Rotate TIMED OUT  Heading=%5.1f", Robot::m_drive.GetHeading());
      break;
      
    default:;
  }

  Robot::m_robotLog.Write(Robot::message);
}

void CmdDriveRotate::Interrupted() {
  m_status = csCancel;
  End();
}
