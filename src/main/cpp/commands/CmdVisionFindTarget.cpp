// Initiate Target search using the Vision subsystem and wait for completion

#include "commands/CmdVisionFindTarget.h"
#include "Robot.h"
#include "RobotMap.h"

CmdVisionFindTarget::CmdVisionFindTarget(Vision::TargetSelect targetSelect) {
  m_targetSelect = targetSelect;
  m_status = csRun;
  m_counter = 0;
}

void CmdVisionFindTarget::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {          // Skip command if in a Group that has been canceled
    m_status = csSkip;
    sprintf(m_message, "Vision:   Find Target SKIP");
  } else if (Robot::m_vision.GetSearchState() == Vision::ssLooking) {           // Set to Done if Vision is already looking
    m_status = csDone;
    sprintf(m_message, "Vision:   Find Target Busy");
  } else {
    m_status = csRun;
    Robot::m_dashboard.SetRobotStatus(rsTargetFound, false);

    if(Robot::m_vision.InTargetMode()) {                                        // Camera already in Target mode
      m_counter = 0;
      Robot::m_vision.FindTarget(m_targetSelect);                               // Initiate target search
    } else {
      m_counter = 6;                                                            // Set change-in-mode wait counter
      Robot::m_vision.SetCameraMode(Vision::cmTarget);                          // Switch camera to Target mode
    }
    Robot::m_robotLog.Write("Vision:   Find Target INIT");
  }
}

void CmdVisionFindTarget::Execute() {
  if(m_status == csRun) {
    if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {        // Cancel if in Group that has been canceled
      m_status = csCancel;
      sprintf(m_message, "Vision:   Find Target CANCELED");
    } else if (m_counter > 0) {                                                 // Waiting for change in Camera mode
      m_counter--;
      if (m_counter == 0) Robot::m_vision.FindTarget(m_targetSelect);           // Wait completed, initiate target search
      
    } else {
      switch (Robot::m_vision.GetSearchState()) {                               // Action dependent on Target Search state
        case Vision::ssNoImage:                                                 
          m_status = csDone;                                                    // If No Image from Camera, set to Done
          sprintf(m_message, "Vision:   No Camera Image");
          if (this->IsParented()) this->GetGroup()->Cancel();                   // If Group command, cancel Group
         break;

        case Vision::ssNoTarget:
          m_status = csDone;                                                    // if No Target found, set to Done
          sprintf(m_message, "Vision:   No Target Found");
          if (this->IsParented()) this->GetGroup()->Cancel();                   // If a Group command, cancel Group
          break;

        case Vision::ssTargetFound:
          m_status = csDone;                                                    // If Target found, set to Done
          Robot::m_dashboard.SetRobotStatus(rsTargetFound, true);
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
  return (m_status != csRun);                                                   // Finished when status is not in Run state
}

void CmdVisionFindTarget::End() {
  Robot::m_robotLog.Write(m_message);
}

void CmdVisionFindTarget::Interrupted() {
  m_status = csCancel;
  sprintf(m_message, "Vision:   Find Target CANCELED");
  End();
}
