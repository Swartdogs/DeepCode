#include "commands/CmdArmSetShoulderDegrees.h"
#include "Robot.h"

CmdArmSetShoulderDegrees::CmdArmSetShoulderDegrees(double degrees) {
  m_degrees = degrees;
}

void CmdArmSetShoulderDegrees::Initialize() {
  Robot::m_arm.SetShoulderPosition(m_degrees);
}

void CmdArmSetShoulderDegrees::Execute() {}

bool CmdArmSetShoulderDegrees::IsFinished() { return true; }

void CmdArmSetShoulderDegrees::End() {}

void CmdArmSetShoulderDegrees::Interrupted() {}
