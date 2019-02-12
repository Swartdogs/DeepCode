#pragma once

#include <frc/commands/Command.h>

class CmdArmSetWristDegrees : public frc::Command {
 public:
  CmdArmSetWristDegrees(double degrees);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private:
  double m_degrees;
};
