#pragma once

#include <frc/commands/Command.h>
#include "subsystems/Elevator.h"
#include "RobotMap.h"

class CmdElevatorDriveFoot : public frc::Command {
 public:
  CmdElevatorDriveFoot(Elevator::FloorSensor floorSensor, double footSpeed, double timeout, int addTime);
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
    int                         m_stopCounter;
    int                         m_addTime;
};
