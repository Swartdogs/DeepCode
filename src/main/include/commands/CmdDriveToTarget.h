#pragma once

#include <frc/commands/Command.h>
#include "RobotMap.h"

class CmdDriveToTarget : public frc::Command {
 public:
  CmdDriveToTarget(double maxSpeed, double timeout, bool hitTarget = false, 
                   double distanceOffset = 0, int postDriveWait = 0);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

private:
  int             m_coastCount;
  double          m_distance;
  double          m_distanceLast;
  double          m_distanceOffset;
  bool            m_driveFinished;
  double          m_heading;
  bool            m_hitTarget;
  double          m_maxSpeed;
  int             m_postDriveWait;
  int             m_powerCount;
  CommandStatus   m_status;
  double          m_timeout;
  int             m_waitCount;
};
