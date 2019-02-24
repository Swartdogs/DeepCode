// Cancel the Robot Climb Sequence

#include "Robot.h"
#include "commands/CmdCancelClimb.h"

CmdCancelClimb::CmdCancelClimb() {}

void CmdCancelClimb::Initialize() {
  Robot::m_elevator.SetCancelClimb(true);
}

void CmdCancelClimb::Execute() {}

bool CmdCancelClimb::IsFinished() { 
  return true; 
}

void CmdCancelClimb::End() {}

void CmdCancelClimb::Interrupted() {}
