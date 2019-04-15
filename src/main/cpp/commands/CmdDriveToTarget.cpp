// Drive Robot using PID control to Target determined by the Vision subsystem
//   and wait for completion or timeout

#include "commands/CmdDriveToTarget.h"
#include "Robot.h"

CmdDriveToTarget::CmdDriveToTarget(double maxSpeed, double timeout, bool hitTarget, 
                                  double distanceOffset, int postDriveWait) {
  Requires(&Robot::m_drive);

  m_maxSpeed        = maxSpeed;
  m_distance        = 0;
  m_distanceLast    = 0;
  m_distanceOffset  = distanceOffset;
  m_driveFinished   = false;
  m_heading         = 0;
  m_hitTarget       = hitTarget;
  m_postDriveWait   = postDriveWait;
  m_timeout         = timeout;
  m_status          = csRun;
  m_waitCount       = 0;
}

void CmdDriveToTarget::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {          // Skip command if in a Group that has been canceled
    m_status = csSkip;

  } else {
    m_status = csRun;
    m_waitCount = 0;
    Robot::m_drive.SetDriveInUse(true);                                         // Set Drive-in-use flag
    Robot::m_drive.SetBrakeMode(true);                                          // Set motor controllers to Brake mode

    m_distance  = Robot::m_vision.GetTargetDistance() + m_distanceOffset; 
    m_heading   = Robot::m_vision.GetTargetAngle() + Robot::m_dashboard.GetDashValue(dvVisionTargetAngle);  
    
    if (m_distance <= 0) {
      Robot::m_drive.RotateInit(m_heading, 0.6, true);
      sprintf(Robot::message, "Vision:   Target Rotate INIT  Heading=%5.1f", m_heading);
    } else {
      m_distanceLast  = 0;                                                       // Initialize drive and rotate PIDs
      m_driveFinished = false;
      
      Robot::m_drive.DriveInit(m_distance, m_heading, m_maxSpeed, 0, true, true); 
      sprintf(Robot::message, "Vision:   Target Drive INIT  Distance=%5.1f Heading=%5.1f", m_distance, m_heading);
    }

    Robot::m_robotLog.Write(Robot::message);

    if (m_timeout > 0) SetTimeout(m_timeout);                                   // Set timeout
  }
}

void CmdDriveToTarget::Execute() {
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

    } else if (m_distance <= 0) {                                               // Rotate Only
      if (Robot::m_drive.RotateIsFinished()) {
        m_status = csDone;
      } else {
        drive = 0;
        rotate = Robot::m_drive.RotateExec();
      }

    } else {                                                                    
      if (Robot::m_drive.DriveIsFinished()) {                                     // Drive has reached target Distance
        m_driveFinished = true;
                                                                                
        if (!m_hitTarget || Robot::m_arm.GetShoulderPosition() == Arm::apLoad) {  // Done if not hitting Target or loading Cargo
          m_status = csDone;
          Robot::m_drive.ArcadeDrive(0, 0);
          return;
        } 
      }

      if (m_waitCount > 0) {
          m_waitCount--;
          if (m_waitCount <= 0) m_status = csStalled;
          
      } else {
        double distanceNow  = Robot::m_drive.GetDistance(Drive::ueCurrentEncoder);
        double driveRate    = distanceNow - m_distanceLast;

        if (distanceNow > (m_distance / 2) && driveRate < 0.05) {                 // Done if hit something
          m_waitCount = m_postDriveWait;
          if (m_waitCount == 0) m_status = csStalled;

        } else if (m_driveFinished) {                                             // Continue forward if distance has been reached
          if (driveRate < 0.5) drive = 0.15;

        } else {                                                                  // Determine drive speed from PID
          drive  = Robot::m_drive.DriveExec();
        }

        rotate = Robot::m_drive.RotateExec();
        m_distanceLast = distanceNow;
      }
    }
  }

  Robot::m_drive.ArcadeDrive(drive, rotate);
}

bool CmdDriveToTarget::IsFinished() { 
  return (m_status != csRun);                                                   // Finished when status is not in Run state
}

void CmdDriveToTarget::End() {
  std::string action = "";
  Robot::m_drive.SetDriveInUse(false);

  switch (m_status) {
    case csSkip:      sprintf(Robot::message, "Vision:   Drive SKIP");  break;
    case csDone:      action = "DONE";        break;
    case csCancel:    action = "CANCELED";    break;
    case csTimedOut:  action = "TIMED OUT";   break;
    case csStalled:   if (m_driveFinished) action = "STALLED (After DONE)";
                      else                 action = "STALLED (Before DONE)";
                      break;
    default:;
  }

  if (action.length() > 0) {
      if (m_distance <= 0) {
        sprintf(Robot::message, "Vision:   Rotate %s  Heading=%5.1f", action.c_str(), Robot::m_drive.GetHeading()); 
      
      } else if (m_postDriveWait > 0 && m_status == csStalled) {
        sprintf(Robot::message, "Vision:   Drive %s  Left=%5.1f  Right = %5.1f  Heading=%5.1f  Wait=%d msec", 
                action.c_str(), Robot::m_drive.GetDistance(Drive::ueLeftEncoder), Robot::m_drive.GetDistance(Drive::ueRightEncoder), 
                Robot::m_drive.GetHeading(), m_postDriveWait * 20);
      
      } else {
        sprintf(Robot::message, "Vision:   Drive %s  Left=%5.1f  Right = %5.1f  Heading=%5.1f", action.c_str(),
                Robot::m_drive.GetDistance(Drive::ueLeftEncoder), Robot::m_drive.GetDistance(Drive::ueRightEncoder), 
                Robot::m_drive.GetHeading());
      }
  }

  Robot::m_robotLog.Write(Robot::message);
}

void CmdDriveToTarget::Interrupted() {
  m_status = csCancel;
  End();
}
