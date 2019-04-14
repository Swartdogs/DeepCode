#pragma once

#include <frc/commands/Command.h>
#include "RobotMap.h"

class CmdAutoStop : public frc::Command {
 public:
  CmdAutoStop(AutoStop condition);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

private:
  AutoStop  m_condition;
};
