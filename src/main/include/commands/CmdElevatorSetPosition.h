#pragma once

#include "subsystems/Elevator.h"
#include <frc/commands/Command.h>
#include "RobotMap.h"

class CmdElevatorSetPosition : public frc::Command {
 public:
  CmdElevatorSetPosition(Elevator::ElevatorPosition position, bool WaitForDone = false, double timeout = 0, double footSpeed = 0,
                          double driveSpeed = 0);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private:
  double                     m_driveSpeed;
  Elevator::ElevatorPosition m_elevatorPosition;
  double                     m_footSpeed;
  CommandStatus              m_status;
  bool                       m_waitForDone;
  double                     m_timeout;
};
