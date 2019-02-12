#include "commands/CmdArmSetManual.h"
#include "Robot.h"

CmdArmSetManual::CmdArmSetManual(bool isManual) {
  m_isManual = isManual;
}

void CmdArmSetManual::Initialize() {
  Robot::m_arm.SetDrivenManually(m_isManual);
}

void CmdArmSetManual::Execute() {}

bool CmdArmSetManual::IsFinished() { return true; }

void CmdArmSetManual::End() {}

void CmdArmSetManual::Interrupted() {}
