// Set the Drive Shifter position to Low or High

#include "commands/CmdDriveSetGear.h"
#include "Robot.h"

CmdDriveSetGear::CmdDriveSetGear(Drive::ShifterPosition position) {
  Requires(&Robot::m_drive);
  m_position  = position;
  m_timeCount = -1;
}

void CmdDriveSetGear::Initialize() {
  m_timeCount = (Robot::m_drive.GetShifterPosition() != m_position) ? -1 : 4;
}

void CmdDriveSetGear::Execute() {
  if (m_timeCount < 4) {
    m_timeCount++;
    if (m_timeCount == 2) Robot::m_drive.SetShifter(m_position);
  }

  Robot::m_drive.ArcadeDrive(0, 0);
}

bool CmdDriveSetGear::IsFinished() { 
  return (m_timeCount >= 4);
}

void CmdDriveSetGear::End() {}

void CmdDriveSetGear::Interrupted() {}
