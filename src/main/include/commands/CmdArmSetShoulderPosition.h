#pragma once

#include <frc/commands/Command.h>
#include "subsystems/Arm.h"

class CmdArmSetShoulderPosition : public frc::Command {
 public:
  CmdArmSetShoulderPosition(Arm::ArmPosition position);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private:
  Arm::ArmPosition m_position;
};
