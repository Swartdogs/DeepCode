#pragma once

#include "subsystems/Arm.h"
#include <frc/commands/Command.h>

class CmdArmSetHatchState : public frc::Command {
 public:
  CmdArmSetHatchState(Arm::HatchState state);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private:
  Arm::HatchState   m_state;
};
