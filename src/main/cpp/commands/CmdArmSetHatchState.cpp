#include "Robot.h"
#include "commands/CmdArmSetHatchState.h"

CmdArmSetHatchState::CmdArmSetHatchState(Arm::HatchState state) {
  m_state = state;
}

void CmdArmSetHatchState::Initialize() {
  Robot::m_arm.SetHatchState(m_state);
}

void CmdArmSetHatchState::Execute() {}

bool CmdArmSetHatchState::IsFinished() { return true; }

void CmdArmSetHatchState::End() {}

void CmdArmSetHatchState::Interrupted() {}
