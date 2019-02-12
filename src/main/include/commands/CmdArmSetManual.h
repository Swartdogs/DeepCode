#pragma once

#include <frc/commands/Command.h>

class CmdArmSetManual : public frc::Command {
 public:
  CmdArmSetManual(bool isManual);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private:
  bool m_isManual;
};
