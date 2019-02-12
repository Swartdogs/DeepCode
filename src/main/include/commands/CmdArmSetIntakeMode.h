#pragma once

#include "subsystems/Arm.h"
#include <frc/commands/Command.h>

class CmdArmSetIntakeMode : public frc::Command {
 public:
  CmdArmSetIntakeMode(Arm::IntakeMode mode);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private:
  Arm::IntakeMode m_mode;
};
