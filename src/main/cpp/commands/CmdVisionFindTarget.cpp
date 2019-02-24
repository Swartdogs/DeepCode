// Initiate Target search using the Vision subsystem and wait for completion

#include "commands/CmdVisionFindTarget.h"
#include "Robot.h"

CmdVisionFindTarget::CmdVisionFindTarget(Vision::TargetSelect targetSelect) {
  m_targetSelect = targetSelect;
  m_status = csRun;
  m_counter = 0;
}

void CmdVisionFindTarget::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
    m_status = csSkip;
    sprintf(m_message, "Vision:   Find Target SKIP");
  } else if (Robot::m_vision.GetSearchState() == Vision::ssLooking) {
    m_status = csDone;
    sprintf(m_message, "Vision:   Find Target Busy");
  } else {
    m_status = csRun;

    if(Robot::m_vision.InTargetMode()) {
      m_counter = 0;
      Robot::m_vision.FindTarget(m_targetSelect);
    } else {
      m_counter = 6;
      Robot::m_vision.SetCameraMode(Vision::cmTarget);
    }
    Robot::m_robotLog.Write("Vision:   Find Target INIT");
  }
}

void CmdVisionFindTarget::Execute() {
  if(m_status == csRun) {
    if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {
      m_status = csCancel;
      sprintf(m_message, "Vision:   Find Target CANCELED");
    } else if (m_counter > 0) {
      m_counter--;
      if (m_counter == 0) Robot::m_vision.FindTarget(m_targetSelect);
      
    } else {
      switch (Robot::m_vision.GetSearchState()) {
        case Vision::ssNoImage:
          m_status = csDone;
          sprintf(m_message, "Vision:   No Camera Image");
          if (this->IsParented()) this->GetGroup()->Cancel();
         break;

        case Vision::ssNoTarget:
          m_status = csDone;
          sprintf(m_message, "Vision:   No Target Found");
          if (this->IsParented()) this->GetGroup()->Cancel();
          break;

        case Vision::ssTargetFound:
          m_status = csDone;
          sprintf(m_message, "Vision:   Target Found at Distance=%5.1f   Heading=%5.1f", 
                  Robot::m_vision.GetTargetDistance(), Robot::m_vision.GetTargetAngle());
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
