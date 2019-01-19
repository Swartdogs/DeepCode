
#pragma once

#include <RobotMap.h>
#include <frc/commands/Command.h>

class CmdDriveRotate : public frc::Command {
 public:
  CmdDriveRotate(double heading, double maxSpeed, bool resetGyro, double timeout = 0);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private: 
  double          m_heading;
  double          m_maxSpeed;
  double          m_timeout;
  bool            m_resetGyro;
  CommandStatus   m_status;
};
