/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

#include <frc/WPILib.h>
#include "RobotMap.h"

class RobotLog {
 public:
  RobotLog(std::string name = "");
  ~RobotLog();

  void        Close();
  void        EndPeriodic();
  RobotMode   GetMode();
  void        LogData();
  std::string ModeName(RobotMode mode);
  void        SetMode(RobotMode mode);
  void        StartPeriodic();
  void        Write(std::string entry, bool includeTime = true);
  void        WriteData(std::string data);
  void        WritePid(std::string output);

private:
  std::string DataString(double value);

  FILE*     m_logoFile;
  FILE*     m_dataFile;
  FILE*     m_pidFile;

  double      m_periodicBeginTime;
  int32_t     m_periodicCount;
  double      m_periodicLastEnd;
  double      m_periodicLastStart;
  double      m_periodicTotalTime;
  RobotMode   m_robotMode;
  std::string m_RobotName;

  char        m_log[100];
};
