/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

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

// Called just before this Command runs the first time
void CmdDriveDistance::Initialize() {
     if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
      m_status = csSkip;
   } else {
     m_status = csRun;
     Robot::m_drive.SetDriveInUse(true);

     Robot::m_drive.DriveInit(m_distance, m_heading, m_maxSpeed, m_minSpeed, m_resetEncoders, m_resetGyro, false);
     if (m_timeout > 0) SetTimeout(m_timeout);
     
     sprintf(Robot::message, "Drive:  Distance INIT  Distance=%5.1f to %5.1f Heading=%5.1f to %5.1f  MaxSpeed=%3.1f", 
             Robot::m_drive.GetDistance(Drive::ueCurrentEncoder), m_distance, Robot::m_drive.GetHeading(), m_heading, m_maxSpeed);

      Robot::m_robotLog.Write(Robot::message);
   }
}

// Called repeatedly when this Command is scheduled to run
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

// Make this return true when this Command no longer needs to run execute()
bool CmdDriveDistance::IsFinished() { 
    return (m_status != csRun); 
}

// Called once after isFinished returns true
void CmdDriveDistance::End() {
  Robot::m_drive.SetDriveInUse(false);

  switch (m_status) {
    case csSkip:
      sprintf(Robot::message, "Drive:   Distance SKIP");
      break;

    case csDone:
      sprintf(Robot::message, "Drive:   Distance DONE   Heading=%5.1f", Robot::m_drive.GetHeading());
      break;

    case csCancel:
      sprintf(Robot::message, "Drive:   Distnace CANCELED   Heading=%5.1f", Robot::m_drive.GetHeading());
      break;

    case csTimedOut:
      sprintf(Robot::message, "Drive:   Distance TIMED OUT   Heading=%5.1f", Robot::m_drive.GetHeading());
      break;
      
    default:;
  }

  Robot::m_robotLog.Write(Robot::message);
}

void CmdDriveDistance::Interrupted() {}
