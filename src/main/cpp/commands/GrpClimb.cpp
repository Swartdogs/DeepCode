#include "Robot.h"
#include "commands/GrpClimb.h"
#include "commands/CmdElevatorSetFoot.h"
#include "commands/CmdElevatorDriveFoot.h"
#include "commands/CmdElevatorSetPosition.h"
#include "commands/CmdDriveDistance.h"
#include "commands/CmdElevatorSetPosition.h"

GrpClimb::GrpClimb(Elevator::ElevatorPosition position) {
  AddSequential(new CmdElevatorSetFoot(Elevator::fpExtended, 1.0));                   // Make sure Foot is extended
  AddSequential(new CmdElevatorSetPosition(position, true));                          // Lift Robot to platform
  AddSequential(new CmdElevatorDriveFoot(Elevator::fsFront, 0.9, 0.1, 1.5, 5));       // Drive Foot until front sensor detects floor
  AddSequential(new CmdElevatorSetFoot(Elevator::fpRetracted, 1.0));                  // Retract Foot
  AddSequential(new CmdElevatorDriveFoot(Elevator::fsRear, 0.9, 0.1, 2.0, 20));       // Drive Foot until rear sensor detects floor
  AddSequential(new CmdElevatorSetPosition(Elevator::epRetracted, true, 0.3, 0.2));   // Move Elevator to retracted while moving Robot forward
  AddSequential(new CmdDriveDistance(6, 0, 0.2, 0, true, true, 2.0));                 // Drive Robot further on the platform
  AddSequential(new CmdElevatorSetFoot(Elevator::fpExtended, 0));                     // Move Foot back to extended
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