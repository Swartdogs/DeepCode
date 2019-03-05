#include "commands/CmdArmSetSpeed.h"
#include "Robot.h"

CmdArmSetSpeed::CmdArmSetSpeed(bool slowSpeed) {
  m_slowSpeed = slowSpeed;
}

void CmdArmSetSpeed::Initialize() {
  Robot::m_arm.SetArmSpeed(m_slowSpeed);
}

void CmdArmSetSpeed::Execute() {}

bool CmdArmSetSpeed::IsFinished() { return true; }

void CmdArmSetSpeed::End() {}

void CmdArmSetSpeed::Interrupted() {}
