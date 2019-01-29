/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc/commands/Command.h>
#include "subsystems/Elevator.h"
#include "RobotMap.h"

class CmdDriveFoot : public frc::Command {
 public:
  CmdDriveFoot(Elevator::PlatformStatus platformStatus, double footSpeed, double timeout);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

  private:
    double                      m_footSpeed;
    Elevator::PlatformStatus    m_platformStatus;
    CommandStatus               m_status;
    double                      m_timeout;
};
