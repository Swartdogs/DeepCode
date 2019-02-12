#include "commands/CmdArmSetShoulderPosition.h"
#include "Robot.h"

CmdArmSetShoulderPosition::CmdArmSetShoulderPosition(Arm::ArmPosition position) {
  m_position = position;
}

void CmdArmSetShoulderPosition::Initialize() {
  Robot::m_arm.SetShoulderPosition(m_position);
}

void CmdArmSetShoulderPosition::Execute() {}

bool CmdArmSetShoulderPosition::IsFinished() { return true; }

void CmdArmSetShoulderPosition::End() {}

void CmdArmSetShoulderPosition::Interrupted() {}
