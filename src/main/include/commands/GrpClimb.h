#pragma once

#include "subsystems/Elevator.h"
#include <frc/commands/CommandGroup.h>
#include "RobotMap.h"

class GrpClimb : public frc::CommandGroup {
 public:
  GrpClimb(Elevator::ElevatorPosition position);
  void Initialize() override;
  void Execute() override;
};
