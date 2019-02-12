#include "commands/CmdArmSetWristDegrees.h"
#include "Robot.h"

CmdArmSetWristDegrees::CmdArmSetWristDegrees(double degrees) {
  m_degrees = degrees;
}

void CmdArmSetWristDegrees::Initialize() {
  Robot::m_arm.SetWristPosition(m_degrees);
}

void CmdArmSetWristDegrees::Execute() {}

bool CmdArmSetWristDegrees::IsFinished() { return true; }

void CmdArmSetWristDegrees::End() {}

void CmdArmSetWristDegrees::Interrupted() {}
