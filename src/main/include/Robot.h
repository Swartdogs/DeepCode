#pragma once

#include <frc/TimedRobot.h>
#include <frc/commands/Command.h>

#include "OI.h"
#include "subsystems/Elevator.h"
#include "subsystems/Drive.h"
#include "subsystems/Vision.h"
#include "subsystems/Arm.h"
#include "RobotLog.h"
#include "Dashboard.h"

class Robot : public frc::TimedRobot {
 public:
  static char   message[100];

  static Dashboard  m_dashboard;
  static Drive      m_drive; 
  static Elevator   m_elevator;
  static Vision     m_vision;
  static Arm        m_arm;
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
  PIDControl                  m_tunePID{"Tune"};
  frc::PowerDistributionPanel m_pdp{};

  cs::UsbCamera               m_cameraDriver{};
  cs::VideoSink               m_cameraServer{};

  void InitializeArm();
  void SetDashRobotValues();
  void StartDriverCamera();
};
