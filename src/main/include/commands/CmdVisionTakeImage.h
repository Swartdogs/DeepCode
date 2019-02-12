#pragma once

#include "subsystems/Vision.h"
#include <frc/commands/Command.h>

class CmdVisionTakeImage : public frc::Command {
 public:
  CmdVisionTakeImage(Vision::TargetSelect targetSelect);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private:
  Vision::TargetSelect  m_targetSelect;
};
