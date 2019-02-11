#pragma once

#include <frc/commands/Command.h>
#include "RobotMap.h"

class CmdDriveToTarget : public frc::Command {
 public:
  CmdDriveToTarget(double timeout);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

private:
  double          m_distance;
  double          m_heading;
  CommandStatus   m_status;
  double          m_timeout;

};
