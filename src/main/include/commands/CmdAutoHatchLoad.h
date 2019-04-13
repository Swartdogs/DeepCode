#pragma once

#include <frc/commands/Command.h>

class CmdAutoHatchLoad : public frc::Command {
 public:
  CmdAutoHatchLoad();
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;
};
