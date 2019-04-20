// Robot Climb sequence for either Platform Level 2 or 3

#include "Robot.h"
#include "commands/GrpClimb.h"
#include "commands/CmdElevatorSetFoot.h"
#include "commands/CmdElevatorDriveFoot.h"
#include "commands/CmdElevatorSetPosition.h"
#include "commands/CmdDriveDistance.h"
#include "commands/CmdElevatorSetPosition.h"
#include "commands/CmdArmSetArmPosition.h"

GrpClimb::GrpClimb(Elevator::ElevatorPosition position) {
  m_position = position;

  AddParallel   (new CmdArmSetArmPosition(Arm::apDefense));
  AddSequential (new CmdElevatorSetFoot(Elevator::fpExtended, 1.0));                         // Make sure Foot is extended
  AddSequential (new CmdElevatorSetPosition(position, true, 4.0));                           // Lift Robot to platform
  AddSequential (new CmdElevatorDriveFoot(Elevator::fsFront, 0.9, 0.1, 3.0, 5));             // Drive Foot until front sensor detects floor
  AddSequential (new CmdElevatorSetFoot(Elevator::fpRetracted, 1.0));                        // Retract Foot
  AddSequential (new CmdElevatorDriveFoot(Elevator::fsRear, 0.9, 0.1, 3.0, 20));             // Drive Foot until rear sensor detects floor
  AddSequential (new CmdElevatorSetPosition(Elevator::epRetracted, true, 4.0, 0.3, 0.2));    // Move Elevator to retracted while moving Robot forward
  AddSequential (new CmdDriveDistance(6, 0, 0.2, 0, true, true, 2.0));                       // Drive Robot further on the platform
  AddSequential (new CmdElevatorSetFoot(Elevator::fpExtended, 0));                           // Move Foot back to extended
}

void GrpClimb::Initialize() {
  double timeRemaining = frc::Timer::GetMatchTime();

  if (timeRemaining < 7.0 && m_position == Elevator::epLevel3) {
    this->Cancel();
    sprintf(Robot::message, "Climb:    %s climb blocked since only %4.1f seconds remain", 
            Robot::m_elevator.GetElevatorPositionName(m_position).c_str(), timeRemaining);
    Robot::m_robotLog.Write(Robot::message);
    Robot::m_dashboard.SetRobotStatus(rsNoClimb, true);

  } else {
    sprintf(Robot::message, "Climb:    %s climb initiated with %4.1f seconds remaining", 
          Robot::m_elevator.GetElevatorPositionName(m_position).c_str(), timeRemaining);
    Robot::m_robotLog.Write(Robot::message);
  
    Robot::m_elevator.SetCancelClimb(false); 
    Robot::m_dashboard.SetRobotStatus(rsClimb, true);
  }                                           // Set Cancel flag to false
}

void GrpClimb::Execute() {
  if (Robot::m_elevator.GetCancelClimb()) {                                           // If Cancel flag is True, cancel Group
    this->Cancel(); 
    if (Robot::m_elevator.SafeToRaise()) {                                            // If safe to raise Elevator, move to Retracted
      Robot::m_elevator.SetElevatorPosition(Elevator::epRetracted);
    } else {                                                                          // If not, keep Elevator at current location
      Robot::m_elevator.SetElevatorSetpoint(Robot::m_elevator.GetElevatorPosition());
    }
  }
}
