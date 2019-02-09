/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "subsystems/Arm.h"
#include "commands/GrpCargoPickup.h"
#include "commands/CmdArmSetArmPosition.h"
#include "commands/CmdArmSetIntakeMode.h"

GrpCargoPickup::GrpCargoPickup() {
  AddParallel (new CmdArmSetArmPosition(Arm::apPickup));
  AddParallel (new CmdArmSetIntakeMode(Arm::imIn));
}
