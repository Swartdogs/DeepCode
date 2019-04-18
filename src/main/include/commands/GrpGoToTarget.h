#pragma once

#include <frc/commands/CommandGroup.h>
#include "subsystems/Arm.h"

class GrpGoToTarget : public frc::CommandGroup {
 public:
  GrpGoToTarget();
  void End() override;
  void Initialize() override;
  void Interrupted() override;
};
