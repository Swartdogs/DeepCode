#pragma once

#include <frc/commands/Command.h>

class CmdVisionToggleMode : public frc::Command {
 public:
  CmdVisionToggleMode();
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;
};
