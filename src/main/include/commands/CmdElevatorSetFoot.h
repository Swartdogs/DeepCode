#pragma once

#include "RobotMap.h"
#include "subsystems/Elevator.h"
#include <frc/commands/Command.h>

class CmdElevatorSetFoot : public frc::Command {
 public:
  CmdElevatorSetFoot(Elevator::FootPosition position, double timeout);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private:
  Elevator::FootPosition m_footPosition;
  double                 m_timeout;
  CommandStatus          m_status;

};
