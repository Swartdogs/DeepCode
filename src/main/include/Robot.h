/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc/TimedRobot.h>
#include <frc/commands/Command.h>
#include <frc/smartdashboard/SendableChooser.h>

#include "OI.h"
#include "subsystems/Elevator.h"
#include "subsystems/Drive.h"
#include "RobotLog.h"
#include "Dashboard.h"

class Robot : public frc::TimedRobot {
 public:
  static char   message[100];

  static Dashboard  m_dashboard;
  static Drive      m_drive; 
  static Elevator   m_elevator;
  static RobotLog   m_robotLog;
  static OI         m_oi;

  void RobotInit() override;
  void RobotPeriodic() override;
  void DisabledInit() override;
  void DisabledPeriodic() override;
  void AutonomousInit() override;
  void AutonomousPeriodic() override;
  void TeleopInit() override;
  void TeleopPeriodic() override;
  void TestInit() override;
  void TestPeriodic() override;

 private:
  frc::SendableChooser<frc::Command*> m_chooser;

  PIDControl  m_tunePID{"Tune"};
};
