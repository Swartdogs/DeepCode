#pragma once

#include <frc/commands/CommandGroup.h>

class GrpLoad : public frc::CommandGroup {
 public:
  GrpLoad();
  void End() override;
  void Interrupted() override;
};