/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/CmdDriveFoot.h"
#include "Robot.h"

CmdDriveFoot::CmdDriveFoot(Elevator::FloorSensor floorSensor, double footSpeed, double timeout) {
  Requires(&Robot::m_drive);

  m_footSpeed       = footSpeed;
  m_floorSensor     = floorSensor;
  m_status          = csRun;
  m_timeout         = fabs(timeout);
}

// Called just before this Command runs the first time
void CmdDriveFoot::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false){
      m_status = csSkip;
  } else {
    m_status = csRun;
    Robot::m_elevator.SetFootInUse(true);
    Robot::m_drive.SetDriveInUse(true);
    if (m_timeout > 0) SetTimeout(m_timeout);

    Robot::m_robotLog.Write("Elevator:   DriveFoot INIT");
  }
}

// Called repeatedly when this Command is scheduled to run
void CmdDriveFoot::Execute() {
  double speed = 0;

  if(m_status == csRun){
      if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false){
        m_status = csCancel;
      } else if (IsTimedOut()) {
        m_status = csTimedOut;
        if (this->IsParented()) this->GetGroup()->Cancel();
      } else if (Robot::m_elevator.FloorDetected(m_floorSensor)) {
        m_status = csDone;
      } else {
        speed = m_footSpeed;
      }
  }

  Robot::m_elevator.DriveFoot(speed);
  Robot::m_drive.ArcadeDrive(speed * Robot::m_dashboard.GetDashValue(dvDriveRatio), 0); 
}

// Make this return true when this Command no longer needs to run execute()
bool CmdDriveFoot::IsFinished() { 
  return (m_status != csRun);
  }

// Called once after isFinished returns true
void CmdDriveFoot::End() {
  Robot::m_elevator.SetFootInUse(false);
  Robot::m_drive.SetDriveInUse(false);

  switch (m_status) {
    case csSkip:
      sprintf(Robot::message, "Elevator:   DriveFoot SKIP");
      break;

    case csDone:
      sprintf(Robot::message, "Elevator:   DriveFoot DONE");
      break;

    case csCancel:
      sprintf(Robot::message, "Elevator:   DriveFoot CANCELED");
      break;

    case csTimedOut:
      sprintf(Robot::message, "Elevator:   DriveFoot TIMED OUT");
      break;
      
    default:;
  }

  Robot::m_robotLog.Write(Robot::message);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CmdDriveFoot::Interrupted() {
  m_status = csCancel;
  End();
}
