/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "commands/GrpClimb.h"
#include "commands/CmdElevatorSetFoot.h"
#include "commands/CmdElevatorReachSetpoint.h"
#include "commands/CmdDriveFoot.h"
#include "commands/CmdElevatorSetPosition.h"
#include "commands/CmdDriveDistance.h"

GrpClimb::GrpClimb() {
  AddSequential(new CmdElevatorSetFoot(Elevator::fpExtended, 1.0));
  AddSequential(new CmdElevatorReachSetpoint());
  AddSequential(new CmdDriveFoot(Elevator::psPartial, 0.3, 4.0));
  AddSequential(new CmdElevatorSetFoot(Elevator::fpRetracted, 1.0));
  AddSequential(new CmdDriveFoot(Elevator::psOn, 0.3, 4.0));
  AddSequential(new CmdElevatorSetPosition(Elevator::epRetracted));
  AddSequential(new CmdElevatorReachSetpoint());
  AddSequential(new CmdDriveDistance(10, 0, 0.4, 0, true, true, 4.0));
}
