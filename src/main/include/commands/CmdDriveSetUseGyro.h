#pragma once

#include <frc/commands/Command.h>

class CmdDriveSetUseGyro : public frc::Command {
 public:
  CmdDriveSetUseGyro(bool useGyro);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

private:
  bool  m_useGyro;
};
