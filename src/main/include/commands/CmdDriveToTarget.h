#pragma once

#include <frc/commands/Command.h>
#include "RobotMap.h"

class CmdDriveToTarget : public frc::Command {
 public:
  CmdDriveToTarget(double addToDistance, double maxSpeed, double timeout);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

private:
  double          m_addToDistance;
  double          m_distance;
  double          m_heading;
  double          m_maxSpeed;
  CommandStatus   m_status;
  double          m_timeout;

};
