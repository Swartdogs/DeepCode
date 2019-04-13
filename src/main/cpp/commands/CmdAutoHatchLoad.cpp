#include "commands/CmdAutoHatchLoad.h"
#include "Robot.h"

CmdAutoHatchLoad::CmdAutoHatchLoad() {
}

void CmdAutoHatchLoad::Initialize() {
  if (this->IsParented()) {                                               // Command is in a Command Group
    if (!this->GetGroup()->IsCanceled()) {                                // Group has not been previously Canceled
      if ((int)Robot::m_dashboard.GetDashValue(dvAutoHatchLoad) == 0) {   // AutoHatchLoad = NO
        this->GetGroup()->Cancel();                                       // Cancel Group
      }
    }
  }
}

void CmdAutoHatchLoad::Execute() {}

bool CmdAutoHatchLoad::IsFinished() { return true; }

void CmdAutoHatchLoad::End() {}

void CmdAutoHatchLoad::Interrupted() {}
