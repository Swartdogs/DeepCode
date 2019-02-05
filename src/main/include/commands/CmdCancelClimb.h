#pragma once

#include <frc/commands/Command.h>

class CmdCancelClimb : public frc::Command {
 public:
  CmdCancelClimb();
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;
};
