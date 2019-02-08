/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "subsystems/Arm.h"
#include <frc/commands/Command.h>

class CmdArmSetIntakeMode : public frc::Command {
 public:
  CmdArmSetIntakeMode(Arm::IntakeMode mode);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

 private:
  Arm::IntakeMode m_mode;
};
