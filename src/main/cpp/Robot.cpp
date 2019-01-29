/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "Robot.h"

#include <frc/commands/Scheduler.h>
#include <frc/smartdashboard/SmartDashboard.h>

char      Robot::message[100];
bool      Robot::driveInUse;

Dashboard Robot::m_dashboard(0,0,1,18);
Drive     Robot::m_drive;
Elevator  Robot::m_elevator;
RobotLog  Robot::m_robotLog("Yeti");
OI        Robot::m_oi;

void Robot::RobotInit() {
  // m_chooser.SetDefaultOption("Default Auto", &m_defaultAuto);
  // m_chooser.AddOption("My Auto", &m_myAuto);
  // frc::SmartDashboard::PutData("Auto Modes", &m_chooser);
}

/**
 * This function is called every robot packet, no matter the mode. Use
 * this for items like diagnostics that you want ran during disabled,
 * autonomous, teleoperated and test.
 *
 * <p> This runs after the mode specific periodic functions, but before
 * LiveWindow and SmartDashboard integrated updating.
 */
void Robot::RobotPeriodic() {}

/**
 * This function is called once each time the robot enters Disabled mode. You
 * can use it to reset any subsystem information you want to clear when the
 * robot is disabled.
 */
void Robot::DisabledInit() {
  Robot::m_robotLog.SetMode(rmDisabled);
  Robot::m_robotLog.Close(); 
}

void Robot::DisabledPeriodic() { frc::Scheduler::GetInstance()->Run(); }

/**
 * This autonomous (along with the chooser code above) shows how to select
 * between different autonomous modes using the dashboard. The sendable chooser
 * code works with the Java SmartDashboard. If you prefer the LabVIEW Dashboard,
 * remove all of the chooser code and uncomment the GetString code to get the
 * auto name from the text box below the Gyro.
 *
 * You can add additional auto modes by adding additional commands to the
 * chooser code above (like the commented example) or additional comparisons to
 * the if-else structure below with additional strings & commands.
 */
void Robot::AutonomousInit() {
    Robot::m_robotLog.SetMode(rmAutonomous);
  // std::string autoSelected = frc::SmartDashboard::GetString(
  //     "Auto Selector", "Default");
  // if (autoSelected == "My Auto") {
  //   m_autonomousCommand = &m_myAuto;
  // } else {
  //   m_autonomousCommand = &m_defaultAuto;
  // }

  // m_autonomousCommand = m_chooser.GetSelected();

  // if (m_autonomousCommand != nullptr) {
  //   m_autonomousCommand->Start();
  // }
}

void Robot::AutonomousPeriodic() { frc::Scheduler::GetInstance()->Run(); }

void Robot::TeleopInit() {
  Robot::m_robotLog.SetMode(rmTeleop);
  // This makes sure that the autonomous stops running when
  // teleop starts running. If you want the autonomous to
  // continue until interrupted by another command, remove
  // this line or comment it out.
  // if (m_autonomousCommand != nullptr) {
  //   m_autonomousCommand->Cancel();
  //   m_autonomousCommand = nullptr;
  // }
}

void Robot::TeleopPeriodic() { 
  Robot::m_robotLog.StartPeriodic();
  frc::Scheduler::GetInstance()->Run(); 
  Robot::m_elevator.Execute(Robot::m_oi.ApplyDeadband(Robot::m_oi.GetDriveJoystickY(), 0.1));
  Robot::m_robotLog.EndPeriodic();
}

void Robot::TestInit() {
  Robot::m_robotLog.SetMode(rmTest);
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
