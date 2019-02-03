#include "Robot.h"

#include <frc/commands/Scheduler.h>
#include <frc/smartdashboard/SmartDashboard.h>

char      Robot::message[100];

RobotLog  Robot::m_robotLog("Yeti");
Dashboard Robot::m_dashboard("2019", 0, 0, 1, 20);       //Dashboard and Log should be created first
Arm       Robot::m_arm; 
Drive     Robot::m_drive;
Vision    Robot::m_vision;
Elevator  Robot::m_elevator;
OI        Robot::m_oi;

void Robot::RobotInit() { 
  m_vision.InitVision();
}

void Robot::RobotPeriodic() {
  m_oi.Periodic();
}

void Robot::DisabledInit() {
  m_robotLog.SetMode(rmDisabled);
  m_robotLog.Close(); 
  m_dashboard.SetRobotMode(rmDisabled);
}

void Robot::DisabledPeriodic() { 
  frc::Scheduler::GetInstance()->Run(); 
  }

void Robot::AutonomousInit() {
  m_robotLog.SetMode(rmAutonomous);
  m_dashboard.SetRobotMode(rmAutonomous);

  // if (m_autonomousCommand != nullptr) {
  //   m_autonomousCommand->Start();
  // }
}

void Robot::AutonomousPeriodic() { 
  frc::Scheduler::GetInstance()->Run(); 
}

void Robot::TeleopInit() {
  m_robotLog.SetMode(rmTeleop);
  m_dashboard.SetRobotMode(rmTeleop);
}

void Robot::TeleopPeriodic() { 
  m_robotLog.StartPeriodic();
  frc::Scheduler::GetInstance()->Run(); 
  m_arm.Periodic();
  m_elevator.Periodic();
  m_robotLog.EndPeriodic();
}

void Robot::TestInit() {
  m_robotLog.SetMode(rmTest);
  m_dashboard.SetRobotMode(rmTest);
}

void Robot::TestPeriodic() {
  static double       setpoint = 0;
  static int          tunePID = -1;
  
  if(m_dashboard.GetDashButton(dbRunPid)) {
      if(tunePID < 0) {
        tunePID   = m_dashboard.GetDashValue(dvPidSelect);
        setpoint  = m_dashboard.GetDashValue(dvPidSetpoint);

        switch(tunePID) {
            case 0:                       //Rotate PID
              m_drive.RotatePidTune();
              break;
            case 1:                       //Drive PID
              m_drive.DrivePidTune();
              m_drive.RotateInit(0, 0.6, true);
              break;
            default:;
        }
        m_tunePID.SetOutputRamp(0.2, 0.05);
        m_tunePID.SetSetpoint(setpoint, 0);
        m_tunePID.SetOutputRange(-m_dashboard.GetDashValue(dvPidMaxOut),m_dashboard.GetDashValue(dvPidMaxOut));

        m_tunePID.SetCoefficient  ('P', m_dashboard.GetDashValue(dvPthreshold),
                                        m_dashboard.GetDashValue(dvPabove),
                                        m_dashboard.GetDashValue(dvPbelow));
                                        
        m_tunePID.SetCoefficient  ('I', m_dashboard.GetDashValue(dvIthreshold),
                                        m_dashboard.GetDashValue(dvIabove),
                                        m_dashboard.GetDashValue(dvIbelow));
                                        
        m_tunePID.SetCoefficient  ('D', m_dashboard.GetDashValue(dvDthreshold),
                                        m_dashboard.GetDashValue(dvDabove),
                                        m_dashboard.GetDashValue(dvDbelow));
        m_tunePID.Reset();
      }

      switch(tunePID) {
        case 0:                                                                 //Rotate PID
          m_drive.ArcadeDrive(0, m_tunePID.Calculate(m_drive.GetHeading()));
          break;
        case 1:                                                                 //Drive PID
          m_drive.ArcadeDrive(m_tunePID.Calculate(m_drive.GetDistance(Drive::ueCurrentEncoder)), m_drive.RotateExec());
          break;
        case 2:                                                                 //Elevator PID
          m_elevator.SetElevatorMotor(m_tunePID.Calculate(m_elevator.GetElevatorPosition()));
          break;
      }
  } else {
    tunePID = -1;
  }
}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
