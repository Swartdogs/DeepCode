// Selects between two maximum speed settings for the Shoulder and Wrist PIDs
//   Used to slow down Shoulder and Wrist movements if a Hatch is not fully captured

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
