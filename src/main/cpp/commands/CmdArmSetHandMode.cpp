// Sets the Arm Hand Mode to Hatch or Cargo

#include "Robot.h"
#include "commands/CmdArmSetHandMode.h"

CmdArmSetHandMode::CmdArmSetHandMode(Arm::HandMode mode) {
  m_mode = mode;
}

void CmdArmSetHandMode::Initialize() {
  Robot::m_arm.SetHandMode(m_mode);
}

void CmdArmSetHandMode::Execute() {}

bool CmdArmSetHandMode::IsFinished() { return true; }

void CmdArmSetHandMode::End() {}

void CmdArmSetHandMode::Interrupted() {}
