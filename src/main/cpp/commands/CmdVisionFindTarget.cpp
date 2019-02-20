#include "commands/CmdVisionFindTarget.h"
#include "Robot.h"

CmdVisionFindTarget::CmdVisionFindTarget(Vision::TargetSelect targetSelect) {
  m_targetSelect = targetSelect;
  m_status = csRun;
}

void CmdVisionFindTarget::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    m_status = csSkip;
    sprintf(m_message, "Vision:   Find Target SKIP");
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
      sprintf(m_message, "Vision:   Find Target CANCELED");
    } else {
      switch (Robot::m_vision.GetSearchState()) {
        case Vision::ssNoTarget:
          m_status = csDone;
          sprintf(m_message, "Vision:   No Target Found");
          if (this->IsParented()) this->GetGroup()->Cancel();
          break;
        case Vision::ssTargetFound:
          m_status = csDone;
          sprintf(m_message, "Vision:   Target Found at Heading=%5.1f  Distance=%5.1f", 
                  Robot::m_vision.GetTargetAngle(), Robot::m_vision.GetTargetDistance());
          break;
        case Vision::ssDone:
          sprintf(m_message, "Vision:   Find Target DONE");
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
  Robot::m_robotLog.Write(m_message);
}

void CmdVisionFindTarget::Interrupted() {
  m_status = csCancel;
  sprintf(m_message, "Vision:   Find Target CANCELED");
  End();
}
