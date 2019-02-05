#include "Robot.h"

#include <frc/commands/Scheduler.h>

char      Robot::message[100];

RobotLog  Robot::m_robotLog("Yeti");
OI        Robot::m_oi;

void Robot::RobotInit() { 

}

void Robot::RobotPeriodic() {

}

void Robot::DisabledInit() {
  m_robotLog.SetMode(rmDisabled);
  m_robotLog.Close(); 
}

void Robot::DisabledPeriodic() { 
  frc::Scheduler::GetInstance()->Run(); 
}

void Robot::AutonomousInit() {
  m_robotLog.SetMode(rmAutonomous);

  // if (m_autonomousCommand != nullptr) {
  //   m_autonomousCommand->Start();
  // }
}

void Robot::AutonomousPeriodic() { 
  frc::Scheduler::GetInstance()->Run(); 
}

void Robot::TeleopInit() {
  m_robotLog.SetMode(rmTeleop);
}

void Robot::TeleopPeriodic() { 
  m_robotLog.StartPeriodic();
  frc::Scheduler::GetInstance()->Run(); 
  m_robotLog.EndPeriodic();
}

void Robot::TestInit() {
  m_robotLog.SetMode(rmTest);
}

void Robot::TestPeriodic() {

}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
