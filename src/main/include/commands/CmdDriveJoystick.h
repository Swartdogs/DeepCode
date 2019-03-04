#pragma once

#include <frc/commands/Command.h>

class CmdDriveJoystick : public frc::Command {
 public:
  CmdDriveJoystick();
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

private:
  bool m_brakesOn;
};
