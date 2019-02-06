#include "Robot.h"

#include <frc/commands/Scheduler.h>

Drive     Robot::m_drive;
OI        Robot::m_oi;

void Robot::RobotInit() { 

}

void Robot::RobotPeriodic() {

}

void Robot::DisabledInit() {

}

void Robot::DisabledPeriodic() { 
  frc::Scheduler::GetInstance()->Run(); 
}

void Robot::AutonomousInit() {
  // if (m_autonomousCommand != nullptr) {
  //   m_autonomousCommand->Start();
  // }
}

void Robot::AutonomousPeriodic() { 
  frc::Scheduler::GetInstance()->Run(); 
}

void Robot::TeleopInit() {

}

void Robot::TeleopPeriodic() { 
  frc::Scheduler::GetInstance()->Run(); 
  m_drive.Periodic();
}

void Robot::TestInit() {

}

void Robot::TestPeriodic() {

}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
