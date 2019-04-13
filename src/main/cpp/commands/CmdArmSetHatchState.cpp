// Sets the state of the Hatch fingers to Grab or Release

#include "Robot.h"
#include "commands/CmdArmSetHatchState.h"

CmdArmSetHatchState::CmdArmSetHatchState(Arm::HatchState state) {
  m_state = state;
}

void CmdArmSetHatchState::Initialize() {
  if ((this->IsParented()) ? !this->GetGroup()->IsCanceled() : true) {    // Execute if not in canceled Group
    Robot::m_arm.SetHatchState(m_state);
  }
}

void CmdArmSetHatchState::Execute() {}

bool CmdArmSetHatchState::IsFinished() { return true; }

void CmdArmSetHatchState::End() {}

void CmdArmSetHatchState::Interrupted() {}
