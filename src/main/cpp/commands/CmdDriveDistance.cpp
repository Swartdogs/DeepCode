// Drive Robot using PID control to a specified Distance at a specified Heading
//   and wait for completion or timeout

#include "commands/CmdDriveDistance.h"
#include "Robot.h"
#include "subsystems/Drive.h"

CmdDriveDistance::CmdDriveDistance(double distance, double heading, double maxSpeed, double minSpeed, bool resetEncoders, 
                                   bool resetGyro, double timeout, bool groupContinue) {
  Requires(&Robot::m_drive);

  m_distance       = distance;
  m_heading        = heading;
  m_maxSpeed       = maxSpeed;
  m_minSpeed       = minSpeed;
  m_resetEncoders  = resetEncoders;
  m_resetGyro      = resetGyro;
  m_timeout        = fabs(timeout);
  m_groupContinue  = groupContinue;
  m_status         = csRun;
}

void CmdDriveDistance::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {          // Skip command if in a Group that has been canceled
    m_status = csSkip;
    
  } else {
    m_status = csRun;
    Robot::m_drive.SetDriveInUse(true);                                         // Set Drive-in-use flag
    Robot::m_drive.SetBrakeMode(true);                                          // Set motor controllers to Brake mode
                                                                                // Initialize drive and rotate PIDs
    Robot::m_drive.DriveInit(m_distance, m_heading, m_maxSpeed, m_minSpeed, m_resetEncoders, m_resetGyro, false);

    if (m_timeout > 0) SetTimeout(m_timeout);                                   // Set timeout 
    
    sprintf(Robot::message, "Drive:    Distance INIT  Distance=%5.1f to %5.1f Heading=%5.1f to %5.1f  MaxSpeed=%3.1f", 
            Robot::m_drive.GetDistance(Drive::ueCurrentEncoder), m_distance, Robot::m_drive.GetHeading(), m_heading, m_maxSpeed);

    Robot::m_robotLog.Write(Robot::message);
  }
}

void CmdDriveDistance::Execute() {
  double drive = 0;
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

    } else if (Robot::m_drive.DriveIsFinished()) {                              // Done if distance has been reached
      m_status = csDone;

    } else {
       drive  = Robot::m_drive.DriveExec();                                     // Get drive and rotate values from PIDs
       rotate = Robot::m_drive.RotateExec();
    }
  }

  Robot::m_drive.ArcadeDrive(drive, rotate);
}

bool CmdDriveDistance::IsFinished() { 
    return (m_status != csRun);                                                 // Finished when status is not in Run state
}

void CmdDriveDistance::End() {
  Robot::m_drive.SetDriveInUse(false);                                          // Clear Drive-in-use flag

  switch (m_status) {
    case csSkip:
      sprintf(Robot::message, "Drive:    Distance SKIP");
      break;

    case csDone:
      sprintf(Robot::message, "Drive:    Distance DONE       Distance Left=%5.1f  Right = %5.1f  Heading=%5.1f", 
              Robot::m_drive.GetDistance(Drive::ueLeftEncoder), Robot::m_drive.GetDistance(Drive::ueRightEncoder), 
              Robot::m_drive.GetHeading());
      break;

    case csCancel:
      sprintf(Robot::message, "Drive:    Distance CANCELED   Distance Left=%5.1f  Right = %5.1f  Heading=%5.1f", 
              Robot::m_drive.GetDistance(Drive::ueLeftEncoder), Robot::m_drive.GetDistance(Drive::ueRightEncoder), 
              Robot::m_drive.GetHeading());
      break;

    case csTimedOut:
      sprintf(Robot::message, "Drive:    Distance TIMED OUT  Distance Left=%5.1f  Right = %5.1f  Heading=%5.1f", 
              Robot::m_drive.GetDistance(Drive::ueLeftEncoder), Robot::m_drive.GetDistance(Drive::ueRightEncoder), 
              Robot::m_drive.GetHeading());
      break;
      
    default:;
  }

  Robot::m_robotLog.Write(Robot::message);
}

void CmdDriveDistance::Interrupted() {
  m_status = csCancel;
  End();
}
