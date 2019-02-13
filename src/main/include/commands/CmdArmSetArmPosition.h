#pragma once

#include <frc/commands/Command.h>
#include "subsystems/Arm.h"
#include "RobotMap.h"

class CmdArmSetArmPosition : public frc::Command {
 public:
  CmdArmSetArmPosition(Arm::ArmPosition position, bool waitForDone = false);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private: 
  Arm::ArmPosition  m_position;
  CommandStatus     m_status;
  bool              m_waitForDone;
};
