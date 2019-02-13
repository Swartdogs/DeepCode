#include "commands/CmdArmSetArmPosition.h"
#include "Robot.h"

CmdArmSetArmPosition::CmdArmSetArmPosition(Arm::ArmPosition position, bool waitForDone) {
  m_position    = position;
  m_status      = csRun; 
  m_waitForDone = waitForDone;
}

void CmdArmSetArmPosition::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    m_status = csCancel;
    Robot::m_robotLog.Write("Arm:      Set Position SKIP");
  } else {
    if (m_waitForDone) {
      m_status = csRun;
    } else {
      m_status = csDone;
    }

    Robot::m_arm.SetShoulderPosition(m_position);
    Robot::m_arm.SetWristPosition(m_position);
  }
}

void CmdArmSetArmPosition::Execute() {
  if (m_status == csRun) {
    if (Robot::m_arm.ShoulderAtSetpoint() && Robot::m_arm.WristAtSetpoint()) m_status = csDone;
  }
}

bool CmdArmSetArmPosition::IsFinished() { 
  return (m_status != csRun); 
}

void CmdArmSetArmPosition::End() {}

void CmdArmSetArmPosition::Interrupted() {}
