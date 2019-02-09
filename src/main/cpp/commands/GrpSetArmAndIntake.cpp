/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/CmdArmSetArmPosition.h"
#include "commands/CmdArmSetIntakeMode.h"
#include "commands/GrpSetArmAndIntake.h"

GrpSetArmAndIntake::GrpSetArmAndIntake(Arm::ArmPosition position, Arm::IntakeMode mode) {
  AddParallel (new CmdArmSetArmPosition(position));
  AddParallel (new CmdArmSetIntakeMode(mode));
}
