#include "commands/CmdArmSetArmPosition.h"
#include "Robot.h"

CmdArmSetArmPosition::CmdArmSetArmPosition(Arm::ArmPosition position) {
  m_position = position; 
}

void CmdArmSetArmPosition::Initialize() {
  Robot::m_arm.SetShoulderPosition(m_position);
  Robot::m_arm.SetWristPosition(m_position);
}

void CmdArmSetArmPosition::Execute() {}

bool CmdArmSetArmPosition::IsFinished() { return true; }

void CmdArmSetArmPosition::End() {}

void CmdArmSetArmPosition::Interrupted() {}
