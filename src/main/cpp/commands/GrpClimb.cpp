#include "Robot.h"
#include "commands/GrpClimb.h"
#include "commands/CmdElevatorSetFoot.h"
#include "commands/CmdElevatorAtSetpoint.h"
#include "commands/CmdElevatorDriveFoot.h"
#include "commands/CmdElevatorSetPosition.h"
#include "commands/CmdDriveDistance.h"
#include "commands/CmdElevatorSetPosition.h"

GrpClimb::GrpClimb(Elevator::ElevatorPosition position) {
  AddSequential(new CmdElevatorSetFoot(Elevator::fpExtended, 1.0));
  AddSequential(new CmdElevatorSetPosition(position));
  AddSequential(new CmdElevatorAtSetpoint());
  AddSequential(new CmdElevatorDriveFoot(Elevator::fsFront, 0.8, 4.0, 2));
  AddSequential(new CmdElevatorSetFoot(Elevator::fpRetracted, 1.0));
  AddSequential(new CmdElevatorDriveFoot(Elevator::fsRear, 0.8, 4.0, 4));
  AddSequential(new CmdElevatorSetPosition(Elevator::epRetracted));
  AddSequential(new CmdElevatorAtSetpoint());
  AddSequential(new CmdDriveDistance(12, 0, 0.3, 0, true, true, 4.0));
}

void GrpClimb::Initialize() {
  Robot::m_elevator.SetCancelClimb(false);
}

void GrpClimb::Execute() {
  if (Robot::m_elevator.GetCancelClimb()) {
    this->Cancel(); 
    if (Robot::m_elevator.SafeToRaise()) {
      Robot::m_elevator.SetElevatorPosition(Elevator::epRetracted);
    } else {
      Robot::m_elevator.SetElevatorSetpoint(Robot::m_elevator.GetElevatorPosition());
    }
  }
}