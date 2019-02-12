#pragma once

#include <frc/commands/Command.h>
#include "subsystems/Arm.h"

class CmdArmSetArmPosition : public frc::Command {
 public:
  CmdArmSetArmPosition(Arm::ArmPosition position);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private: 
  Arm::ArmPosition m_position;
};
