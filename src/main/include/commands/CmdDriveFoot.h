#pragma once

#include <frc/commands/Command.h>
#include "subsystems/Elevator.h"
#include "RobotMap.h"

class CmdDriveFoot : public frc::Command {
 public:
  CmdDriveFoot(Elevator::FloorSensor floorSensor, double footSpeed, double timeout);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

  private:
    double                      m_footSpeed;
    Elevator::FloorSensor       m_floorSensor;
    CommandStatus               m_status;
    double                      m_timeout;
};
