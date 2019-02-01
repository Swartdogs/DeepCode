#include "Robot.h"

#include <frc/commands/Scheduler.h>
#include <frc/smartdashboard/SmartDashboard.h>

char      Robot::message[100];

RobotLog  Robot::m_robotLog("Yeti");
Dashboard Robot::m_dashboard(0,0,1,20);       //Dashboard and Log should be created first

Drive     Robot::m_drive;
Elevator  Robot::m_elevator;
OI        Robot::m_oi;

void Robot::RobotInit() { 
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
            case 0: 
            break;
            case 1:
            break;
            case 2:
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
        case 0:
        
        break;
        case 1:
        break;
        case 2:
        break;
      }
  }
}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
