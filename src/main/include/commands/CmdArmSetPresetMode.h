#pragma once

#include <frc/commands/Command.h>

class CmdArmSetPresetMode : public frc::Command {
 public:
  CmdArmSetPresetMode(bool usePreset);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;
 private:
  bool m_usePreset;
};
