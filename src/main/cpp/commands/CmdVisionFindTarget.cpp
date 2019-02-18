#include "commands/CmdVisionFindTarget.h"
#include "Robot.h"

CmdVisionFindTarget::CmdVisionFindTarget(Vision::TargetSelect targetSelect) {
  m_targetSelect = targetSelect;
  m_status = csRun;
}

void CmdVisionFindTarget::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    m_status = csSkip;
  } else {
    m_status = csRun;
    Robot::m_robotLog.Write("Vision:   Find Target INIT");
    Robot::m_vision.FindTarget(m_targetSelect);
  }
}

void CmdVisionFindTarget::Execute() {
  if(m_status == csRun) {
    if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
      m_status = csCancel;
    } else {
      switch (Robot::m_vision.GetSearchState()) {
        case Vision::ssNoTarget:
          m_status = csDone;
          if (this->IsParented()) this->GetGroup()->Cancel();
          break;
        case Vision::ssTargetFound:
        case Vision::ssDone:
          m_status = csDone;
          break;
      }
    }
  }
}

bool CmdVisionFindTarget::IsFinished() { 
  return (m_status != csRun); 
}

void CmdVisionFindTarget::End() {
  switch (m_status) {
    case csSkip:    sprintf(Robot::message, "Vision:   Find Target SKIP");      break;
    case csDone:    sprintf(Robot::message, "Vision:   Find Target DONE");      break;
    case csCancel:  sprintf(Robot::message, "Vision:   Find Target CANCELED");  break;
    default:;
  }

  Robot::m_robotLog.Write(Robot::message);
}

void CmdVisionFindTarget::Interrupted() {
  m_status = csCancel;
  End();
}
