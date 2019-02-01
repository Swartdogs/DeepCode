#include "Robot.h"
#include "commands/GrpClimb.h"
#include "commands/CmdElevatorSetFoot.h"
#include "commands/CmdElevatorReachSetpoint.h"
#include "commands/CmdDriveFoot.h"
#include "commands/CmdElevatorSetPosition.h"
#include "commands/CmdDriveDistance.h"
#include "commands/CmdElevatorSetPosition.h"

GrpClimb::GrpClimb(Elevator::ElevatorPosition position) {
  AddSequential(new CmdElevatorSetFoot(Elevator::fpExtended, 1.0));
  AddSequential(new CmdElevatorSetPosition(position));
  AddSequential(new CmdElevatorReachSetpoint());
  AddSequential(new CmdDriveFoot(Elevator::fsFront, 0.3, 4.0));
  AddSequential(new CmdElevatorSetFoot(Elevator::fpRetracted, 1.0));
  AddSequential(new CmdDriveFoot(Elevator::fsRear, 0.3, 4.0));
  AddSequential(new CmdElevatorSetPosition(Elevator::epRetracted));
  AddSequential(new CmdElevatorReachSetpoint());
  AddSequential(new CmdDriveDistance(10, 0, 0.4, 0, true, true, 4.0));
}

void GrpClimb::Initialize() {
  Robot::m_elevator.SetCancelClimb(false);
}

void GrpClimb::Execute() {
  if (Robot::m_elevator.GetCancelClimb()) this->Cancel();
}