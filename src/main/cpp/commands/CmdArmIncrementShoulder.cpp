// Increments or Decrements the Arm Shoulder Setpoint by a specified number of degrees

#include "commands/CmdArmIncrementShoulder.h"
#include "Robot.h"

CmdArmIncrementShoulder::CmdArmIncrementShoulder(double degrees) {
  m_degrees = degrees;
}

void CmdArmIncrementShoulder::Initialize() {
  double shoulderNow = Robot::m_arm.GetShoulderSetpoint();

  sprintf(Robot::message, "Shoulder: Change setpoint from %5.1f to %5.1f", shoulderNow, shoulderNow + m_degrees);
  Robot::m_robotLog.Write(Robot::message);

  Robot::m_arm.SetShoulderPosition(shoulderNow + m_degrees, Arm::apUnknown, false);
}

void CmdArmIncrementShoulder::Execute() {}

bool CmdArmIncrementShoulder::IsFinished() { return true; }

void CmdArmIncrementShoulder::End() {}

void CmdArmIncrementShoulder::Interrupted() {}
