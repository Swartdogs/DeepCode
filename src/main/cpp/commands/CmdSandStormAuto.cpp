#include "commands/CmdSandStormAuto.h"
#include "Robot.h"

CmdSandStormAuto::CmdSandStormAuto() {
}

void CmdSandStormAuto::Initialize() {
  if (!Robot::m_oi.SandStormAutoRunning()) Robot::m_oi.SandStormAutoInit();
}

void CmdSandStormAuto::Execute() {}

bool CmdSandStormAuto::IsFinished() { return true; }

void CmdSandStormAuto::End() {}

void CmdSandStormAuto::Interrupted() {}
