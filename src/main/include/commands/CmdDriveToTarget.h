#pragma once

#include <frc/commands/Command.h>
#include "RobotMap.h"

class CmdDriveToTarget : public frc::Command {
 public:
  CmdDriveToTarget(double maxSpeed, double timeout, bool hitTarget = false);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

private:
  double          m_distance;
  double          m_distanceLast;
  bool            m_driveFinished;
  double          m_heading;
  bool            m_hitTarget;
  double          m_maxSpeed;
  CommandStatus   m_status;
  double          m_timeout;

};
