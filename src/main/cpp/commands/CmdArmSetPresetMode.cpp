#include "Robot.h"
#include "commands/CmdArmSetPresetMode.h"

CmdArmSetPresetMode::CmdArmSetPresetMode(bool usePreset) {
  m_usePreset = usePreset;
}

void CmdArmSetPresetMode::Initialize() {
  Robot::m_arm.SetPresetMode(m_usePreset);
}

void CmdArmSetPresetMode::Execute() {}

bool CmdArmSetPresetMode::IsFinished() { return true; }

void CmdArmSetPresetMode::End() {}

void CmdArmSetPresetMode::Interrupted() {}
