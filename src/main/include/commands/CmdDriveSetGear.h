#pragma once

#include "subsystems/Drive.h"
#include <frc/commands/Command.h>

class CmdDriveSetGear : public frc::Command {
 public:
  CmdDriveSetGear(Drive::ShifterPosition position);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private: 
  Drive::ShifterPosition  m_position;
  int                     m_timeCount;
};
