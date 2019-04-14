#include "commands/CmdAutoStop.h"
#include "Robot.h"

CmdAutoStop::CmdAutoStop(AutoStop condition) {
  m_condition = condition;
}

void CmdAutoStop::Initialize() {
  if (this->IsParented()) {                                               // Command is in a Command Group
    if (!this->GetGroup()->IsCanceled()) {                                // Group has not been previously Canceled
      if ((int)Robot::m_dashboard.GetDashValue(dvAutoStop) == m_condition) {   // AutoHatchLoad = NO
        this->GetGroup()->Cancel();                                       // Cancel Group
      }
    }
  }
}

void CmdAutoStop::Execute() {}

bool CmdAutoStop::IsFinished() { return true; }

void CmdAutoStop::End() {}

void CmdAutoStop::Interrupted() {}
