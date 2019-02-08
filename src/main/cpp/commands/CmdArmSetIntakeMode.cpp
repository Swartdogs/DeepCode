#include "Robot.h"
#include "commands/CmdArmSetIntakeMode.h"

CmdArmSetIntakeMode::CmdArmSetIntakeMode(Arm::IntakeMode mode) {
  m_mode = mode;
}

void CmdArmSetIntakeMode::Initialize() {
  Robot::m_arm.SetIntakeMode(m_mode);
}

void CmdArmSetIntakeMode::Execute() {}

bool CmdArmSetIntakeMode::IsFinished() { return true; }

void CmdArmSetIntakeMode::End() {}

void CmdArmSetIntakeMode::Interrupted() {}
