#include "Robot.h"

#include <frc/commands/Scheduler.h>
#include <frc/smartdashboard/SmartDashboard.h>

char      Robot::message[100];

Dashboard Robot::m_dashboard("2019", 0, 17, 1, 40);       //Dashboard and Log should be created first
OI        Robot::m_oi;

void Robot::RobotInit() { 

}

void Robot::RobotPeriodic() {

}

void Robot::DisabledInit() {
  m_dashboard.SetRobotMode(rmDisabled);
}

void Robot::DisabledPeriodic() { 
  frc::Scheduler::GetInstance()->Run(); 
}

void Robot::AutonomousInit() {
  m_dashboard.SetRobotMode(rmAutonomous);
}

void Robot::AutonomousPeriodic() { 
  frc::Scheduler::GetInstance()->Run(); 
}

void Robot::TeleopInit() {
  m_dashboard.SetRobotMode(rmTeleop);
}

void Robot::TeleopPeriodic() { 
  frc::Scheduler::GetInstance()->Run(); 
}

void Robot::TestInit() {
  m_dashboard.SetRobotMode(rmTest);
}

void Robot::TestPeriodic() {

}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
