#pragma once

#include <frc/commands/Command.h>

class CmdArmSetSpeed : public frc::Command {
 public:
  CmdArmSetSpeed(bool slowSpeed);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;
 private:
  bool m_slowSpeed;
};
