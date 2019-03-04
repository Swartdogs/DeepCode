// Drive Robot using PID control to Target determined by the Vision subsystem
//   and wait for completion or timeout

#include "commands/CmdDriveToTarget.h"
#include "Robot.h"

CmdDriveToTarget::CmdDriveToTarget(double maxSpeed, double timeout) {
  Requires(&Robot::m_drive);

  m_maxSpeed      = maxSpeed;
  m_distance      = 0;
  m_heading       = 0;
  m_timeout       = timeout;
  m_status        = csRun;
}

void CmdDriveToTarget::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {          // Skip command if in a Group that has been canceled
    m_status = csSkip;
  } else {
    m_status = csRun;
    Robot::m_drive.SetDriveInUse(true);                                         // Set Drive-in-use flag
    Robot::m_drive.SetBrakeMode(true);                                          // Set motor controllers to Brake mode

    m_distance  = Robot::m_vision.GetTargetDistance();                          // Get Distance and Angle from Vision and add adjustments
    m_heading   = Robot::m_vision.GetTargetAngle() + Robot::m_dashboard.GetDashValue(dvVisionAngleOffset);  
                                                                                // Initialize drive and rotate PIDs

    if (Robot::m_arm.GetHandMode() == Arm::hmHatch) {
      m_distance += Robot::m_dashboard.GetDashValue(dvVisionHatchOffset);
    } else {
      m_distance += Robot::m_dashboard.GetDashValue(dvVisionCargoOffset);
    }

    Robot::m_drive.DriveInit(m_distance, m_heading, m_maxSpeed, 0, true, true);

    if (m_timeout > 0) SetTimeout(m_timeout);                                   // Set timeout
    
    sprintf(Robot::message, "Drive:    To Target INIT  Distance=%5.1f Heading=%5.1f", m_distance, m_heading);
    Robot::m_robotLog.Write(Robot::message);
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
    } else if (Robot::m_drive.DriveIsFinished()) {                              // Done if distance has been reached
      m_status = csDone;
    } else {                                                                    // Get drive and rotate values from PIDs
       drive  = Robot::m_drive.DriveExec();
       rotate = Robot::m_drive.RotateExec();
    }
  }

  Robot::m_drive.ArcadeDrive(drive, rotate);
}

bool CmdDriveToTarget::IsFinished() { 
  return (m_status != csRun);                                                   // Finished when status is not in Run state
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
