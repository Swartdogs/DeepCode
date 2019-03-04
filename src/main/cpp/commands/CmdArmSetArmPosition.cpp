// Sets the Arm Shoulder and Wrist setpoints to a specified Arm Position
//   Optionally waits for the Shoulder and Arm to reach those setpoints

#include "commands/CmdArmSetArmPosition.h"
#include "Robot.h"

CmdArmSetArmPosition::CmdArmSetArmPosition(Arm::ArmPosition position, bool waitForDone) {
  m_position    = position;
  m_status      = csRun; 
  m_waitForDone = waitForDone;
}

void CmdArmSetArmPosition::Initialize() {
  if ((this->IsParented()) ? this->GetGroup()->IsCanceled() : false) {        // Skip command if in a Group that has been canceled
    m_status = csCancel;
    Robot::m_robotLog.Write("Arm:      Set Position SKIP");
  } else {                                                                    
    if (m_waitForDone) {                                                      // Set status to Run if waiting for completion
      m_status = csRun;
    } else {                                                                  // Set status to Done if not waiting
      m_status = csDone;
    }

    Arm::ArmPosition newPosition = m_position;
    
    if (newPosition == Arm::apByHand) {                                        // Determine Position based on Hand Mode
      if (Robot::m_arm.GetHandMode() == Arm::hmHatch) {
        newPosition = Arm::apLow;
      } else {
        newPosition = Arm::apLoad;
      }
    }

    Robot::m_arm.SetArmPosition(newPosition);
  }
}

void CmdArmSetArmPosition::Execute() {
  if (m_status == csRun) {                                                    // Done if Shoulder and Wrist at setpoints
    if (Robot::m_arm.ShoulderAtSetpoint() && Robot::m_arm.WristAtSetpoint()) m_status = csDone;
  }
}

bool CmdArmSetArmPosition::IsFinished() { 
  return (m_status != csRun); 
}

void CmdArmSetArmPosition::End() {}

void CmdArmSetArmPosition::Interrupted() {}
