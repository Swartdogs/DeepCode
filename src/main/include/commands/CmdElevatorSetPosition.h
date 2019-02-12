#pragma once

#include "subsystems/Elevator.h"
#include <frc/commands/Command.h>
#include "RobotMap.h"

class CmdElevatorSetPosition : public frc::Command {
 public:
  CmdElevatorSetPosition(Elevator::ElevatorPosition position, bool WaitForDone = false, double footSpeed = 0,
                          double driveSpeed = 0);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private:
  Elevator::ElevatorPosition m_elevatorPosition;
  CommandStatus              m_status;
  bool                       m_waitForDone;
  double                     m_footSpeed;
  double                     m_driveSpeed;
};
