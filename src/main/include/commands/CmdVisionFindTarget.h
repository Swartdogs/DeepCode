#pragma once

#include <frc/commands/Command.h>
#include "subsystems/Vision.h"
#include "RobotMap.h"

class CmdVisionFindTarget : public frc::Command {
 public:
  CmdVisionFindTarget(Vision::TargetSelect targetSelect = Vision::tsBest);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

private:
  Vision::TargetSelect  m_targetSelect;
  CommandStatus         m_status;
  char                  m_message[100];
};
