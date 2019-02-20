#include "commands/CmdArmIncrementShoulder.h"
#include "Robot.h"

CmdArmIncrementShoulder::CmdArmIncrementShoulder(double degrees) {
  m_degrees = degrees;
}

void CmdArmIncrementShoulder::Initialize() {
  Robot::m_arm.SetShoulderPosition(m_degrees + Robot::m_arm.GetShoulderSetpoint());
}

void CmdArmIncrementShoulder::Execute() {}

bool CmdArmIncrementShoulder::IsFinished() { return true; }

void CmdArmIncrementShoulder::End() {}

void CmdArmIncrementShoulder::Interrupted() {}
