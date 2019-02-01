#pragma once

#include "subsystems/Elevator.h"
#include <frc/commands/Command.h>

class CmdElevatorSetPosition : public frc::Command {
 public:
  CmdElevatorSetPosition(Elevator::ElevatorPosition position);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private:
  Elevator::ElevatorPosition m_elevatorPosition;
};
