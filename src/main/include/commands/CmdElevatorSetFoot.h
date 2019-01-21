/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "subsystems/Elevator.h"
#include <frc/commands/Command.h>

class CmdElevatorSetFoot : public frc::Command {
 public:
  CmdElevatorSetFoot(Elevator::FootPosition position);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private:
  Elevator::FootPosition m_footPosition;
};
