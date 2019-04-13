#pragma once

#include <frc/commands/Command.h>

class CmdSandStormAuto : public frc::Command {
 public:
  CmdSandStormAuto();
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;
};
