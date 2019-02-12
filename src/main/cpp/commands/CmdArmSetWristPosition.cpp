#include "commands/CmdArmSetWristPosition.h"
#include "Robot.h"

CmdArmSetWristPosition::CmdArmSetWristPosition(Arm::ArmPosition position) {
  m_position = position;
}

void CmdArmSetWristPosition::Initialize() {
  Robot::m_arm.SetWristPosition(m_position);
}

void CmdArmSetWristPosition::Execute() {}

bool CmdArmSetWristPosition::IsFinished() { return true; }

void CmdArmSetWristPosition::End() {}

void CmdArmSetWristPosition::Interrupted() {}
