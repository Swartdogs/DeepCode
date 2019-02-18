#pragma once

#include <frc/commands/Command.h>

class CmdArmIncrementShoulder : public frc::Command {
 public:
  CmdArmIncrementShoulder(double degrees);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

  private:
  double m_degrees;
};
