/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include "subsystems/Arm.h"
#include <frc/commands/CommandGroup.h>

class GrpSetArmAndIntake : public frc::CommandGroup {
 public:
  GrpSetArmAndIntake(Arm::ArmPosition position, Arm::IntakeMode mode);
};
