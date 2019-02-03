#pragma once

#include <frc/commands/Command.h>

class CmdElevatorAtSetpoint : public frc::Command {
 public:
  CmdElevatorAtSetpoint();
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;
};
