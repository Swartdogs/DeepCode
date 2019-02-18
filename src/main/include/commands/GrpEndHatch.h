#pragma once

#include <frc/commands/CommandGroup.h>
#include "subsystems/Vision.h"

class GrpEndHatch : public frc::CommandGroup {
 public:
  GrpEndHatch(Vision::TargetSelect target);
};
