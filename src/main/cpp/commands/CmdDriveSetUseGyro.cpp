#include "commands/CmdDriveSetUseGyro.h"
#include "Robot.h"

CmdDriveSetUseGyro::CmdDriveSetUseGyro(bool useGyro) {
  m_useGyro = useGyro;
}

void CmdDriveSetUseGyro::Initialize() {
  Robot::m_drive.SetUseGyro(m_useGyro);
}

void CmdDriveSetUseGyro::Execute() {}

bool CmdDriveSetUseGyro::IsFinished() { return true; }

void CmdDriveSetUseGyro::End() {}

void CmdDriveSetUseGyro::Interrupted() {}
