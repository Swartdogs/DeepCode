#pragma once

#include "subsystems/Arm.h"
#include <frc/commands/Command.h>

class CmdArmSetHandMode : public frc::Command {
 public:
  CmdArmSetHandMode(Arm::HandMode mode);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private:
  Arm::HandMode   m_mode;
};
