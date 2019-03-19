// Toggle the Camera mode between Target and Driver

#include "commands/CmdVisionToggleMode.h"
#include "Robot.h"

CmdVisionToggleMode::CmdVisionToggleMode() {
}

void CmdVisionToggleMode::Initialize() {
  if (Robot::m_vision.InTargetMode()) {                                         // Toggle mode depending on current mode
    Robot::m_vision.SetCameraMode(Vision::cmDriver);
  } else {
    Robot::m_vision.SetCameraMode(Vision::cmTarget);
  }
}

void CmdVisionToggleMode::Execute() {}

bool CmdVisionToggleMode::IsFinished() { return true; }

void CmdVisionToggleMode::End() {}

void CmdVisionToggleMode::Interrupted() {}