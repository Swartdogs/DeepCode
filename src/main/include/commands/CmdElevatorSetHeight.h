#pragma once

#include <frc/commands/Command.h>

class CmdElevatorSetHeight : public frc::Command {
 public:
  CmdElevatorSetHeight(double inches);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private:
  double m_inches;
};
