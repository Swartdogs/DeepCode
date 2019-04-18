#include "commands/CmdArmSetPresetPosition.h"
#include "Robot.h"

CmdArmSetPresetPosition::CmdArmSetPresetPosition(Arm::ArmPosition position) {
  m_position = position;
}

void CmdArmSetPresetPosition::Initialize() {
  Robot::m_arm.SetPresetPosition(m_position);  
}

void CmdArmSetPresetPosition::Execute() {}

bool CmdArmSetPresetPosition::IsFinished() { return true; }

void CmdArmSetPresetPosition::End() {}

void CmdArmSetPresetPosition::Interrupted() {}
