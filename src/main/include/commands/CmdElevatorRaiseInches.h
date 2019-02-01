#pragma once

#include <frc/commands/Command.h>

class CmdElevatorRaiseInches : public frc::Command {
 public:
  CmdElevatorRaiseInches(double inches);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private:
  double m_inches;
};
