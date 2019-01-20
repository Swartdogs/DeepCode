/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc/commands/Command.h>
#include "RobotMap.h"


class CmdDriveDistance : public frc::Command {
 public:
  CmdDriveDistance(double distance, double heading, double maxSpeed, double minSpeed, bool resetEncoders, 
                   bool resetGyro, double timeout = 0, bool groupContinue = false);
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;

  private:
    double          m_distance;
    bool            m_groupContinue;
    double          m_heading;
    double          m_maxSpeed;
    double          m_minSpeed;
    bool            m_resetEncoders;
    bool            m_resetGyro;
    CommandStatus   m_status;
    double          m_timeout;
};
