// Drive Robot using PID control to Target determined by the Vision subsystem
//   and wait for completion or timeout

#include "commands/CmdDriveToTarget.h"
#include "Robot.h"

CmdDriveToTarget::CmdDriveToTarget(double maxSpeed, double timeout, bool hitTarget, 
                                  double distanceOffset, bool loadHatch) {
  Requires(&Robot::m_drive);

  m_coastCount      = 0;
  m_distance        = 0;
  m_distanceLast    = 0;
  m_distanceOffset  = distanceOffset;
  m_driveFinished   = false;
  m_driveSpeed      = 0;
  m_heading         = 0;
  m_hitTarget       = hitTarget;
  m_loadHatch       = loadHatch;
  m_loadState       = 0;
  m_maxSpeed        = maxSpeed;
  m_powerCount      = 0;
  m_timeout         = timeout;
  m_status          = csRun;
}

void CmdDriveToTarget::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {          // Skip command if in a Group that has been canceled
    m_status = csSkip;

  } else {
    m_status = csRun;
    Robot::m_drive.SetDriveInUse(true);                                         // Set Drive-in-use flag
    Robot::m_drive.SetBrakeMode(true);                                          // Set motor controllers to Brake mode

    m_distance  = Robot::m_vision.GetTargetDistance() + m_distanceOffset; 
    m_heading   = Robot::m_vision.GetTargetAngle() + Robot::m_dashboard.GetDashValue(dvVisionTargetAngle) + 
                    Robot::m_drive.GetHeading();  
    
    if (m_distance <= 0) {
      Robot::m_drive.RotateInit(m_heading, 0.6, false, true);
      sprintf(Robot::message, "Vision:   Rotate INIT  Heading=%5.1f", m_heading);
    } else {
      m_distanceLast  = 0;                                                      // Initialize drive and rotate PIDs
      m_driveFinished = false;
      m_driveSpeed    = 0;
      m_loadState     = 0;
      m_coastCount    = 0;                                                      // Reset counters
      m_powerCount    = 0;
      
      Robot::m_drive.DriveInit(m_distance, m_heading, m_maxSpeed, 0, true, false); 

      if (m_loadHatch) {
        sprintf(Robot::message, "Vision:   Distance INIT  Distance=%5.1f Heading=%5.1f (Load Hatch)", m_distance, m_heading);
      } else {
        sprintf(Robot::message, "Vision:   Distance INIT  Distance=%5.1f Heading=%5.1f", m_distance, m_heading);
      }
    }

    Robot::m_robotLog.Write(Robot::message);

    if (m_timeout > 0) SetTimeout(m_timeout);                                   // Set timeout
  }
}

void CmdDriveToTarget::Execute() {
  double drive  = 0;
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
      if (Robot::m_drive.DriveIsFinished()) {                                   // Drive has reached target Distance
        m_driveFinished = true;                                                 // Set Drive Finished flag

        if (!m_hitTarget || Robot::m_arm.GetShoulderPosition() == Arm::apLoad) {  // Done if not hitting Target or loading Cargo
          m_status = csDone;
          Robot::m_drive.ArcadeDrive(0, 0);
          return;
        } 
      }

      double distanceNow    = Robot::m_drive.GetDistance(Drive::ueCurrentEncoder);
      double distanceChange = distanceNow - m_distanceLast;                     // Change in Distance

      if (m_loadState > 0) {                                                    // Hatch Load Sequence
        switch (m_loadState) {
          case 1: if (Robot::m_arm.WristAtSetpoint()) {                         // Wait for Wrist to be at Setpoint
                    Robot::m_arm.SetHatchState(Arm::hsGrab);                    // Grab Hatch
                    m_driveSpeed = 0.1;                                         // Start to back up
                    m_loadState = 2;
                    m_distanceLast = distanceNow;
                  }
                  break;
          case 2: if (distanceChange < -6.0) {                                  // Backed up 6"
                    m_driveSpeed = 0;                                           // Command finished
                    m_status = csStalled;
                  } else if (m_driveSpeed > -0.75) {
                    m_driveSpeed -= 0.05;                                       // Increase backup speed
                  }
                  break;
          default:;
        }

        drive = m_driveSpeed;

      } else {
        if (distanceNow > (m_distance * 0.75) && distanceChange < 0.05) {       // Robot Hit target
          if (m_loadHatch) m_loadState = 1;                                     // Start Hatch Load Sequence
          else             m_status = csStalled;                                // If not loading, Command is finished

        } else if (m_driveFinished) {                                           // Drive Finished - Waiting for Hit
          if (distanceChange < 0.6) {                                           // Low Rate-of-Change
            drive = 0.15;                                                       // Apply power
            m_powerCount++;
          } else {                                                              // Coast
            m_coastCount++;
          }

        } else {                                                                // Determine drive speed from PID
          drive  = Robot::m_drive.DriveExec();
        }

        rotate = Robot::m_drive.RotateExec();                                   // Determine rotoation speed from PID
        m_distanceLast = distanceNow;                                           // Save current distance
      }
    }
  }

  Robot::m_drive.ArcadeDrive(drive, rotate);
}

bool CmdDriveToTarget::IsFinished() { 
  return (m_status != csRun);                                                   // Finished when status is not Run
}

void CmdDriveToTarget::End() {
  std::string action = "";
  Robot::m_drive.SetDriveInUse(false);

  switch (m_status) {
    case csSkip:      sprintf(Robot::message, "Vision:   Drive SKIP");  break;
    case csDone:      action = "DONE";        break;
    case csCancel:    action = "CANCELED";    break;
    case csTimedOut:  action = "TIMED OUT";   break;
    case csStalled:   if (m_driveFinished) {
                        sprintf(Robot::message, "STALLED (C=%d P=%d)", m_coastCount, m_powerCount);
                        action = std::string(Robot::message);
                      } else {
                        action = "STALLED (During)";                        
                      }  
                      break;
    default:;
  }

  if (action.length() > 0) {
      if (m_distance <= 0) {
        sprintf(Robot::message, "Vision:   Rotate %s  Heading=%5.1f", action.c_str(), Robot::m_drive.GetHeading()); 
      } else {
        sprintf(Robot::message, "Vision:   Distance %s  Left=%5.1f  Right = %5.1f  Heading=%5.1f", action.c_str(),
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
