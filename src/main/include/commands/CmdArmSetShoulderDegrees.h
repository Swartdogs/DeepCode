#pragma once

#include <frc/commands/Command.h>

class CmdArmSetShoulderDegrees : public frc::Command {
 public:
  CmdArmSetShoulderDegrees(double degrees);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;
 
 private:
  double m_degrees;
};
