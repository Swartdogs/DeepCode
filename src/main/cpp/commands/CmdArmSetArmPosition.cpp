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
    
    if (newPosition == Arm::apPreset) {                                       // Use Preset Position
      newPosition = Robot::m_arm.GetPresetPosition();                         // Get Preset Position
      double distanceOffset = 0;                                                

      switch (newPosition) {
        case Arm::apLow:    distanceOffset = Robot::m_dashboard.GetDashValue(dvVisionHatchLow);   break;
        case Arm::apMid:    distanceOffset = Robot::m_dashboard.GetDashValue(dvVisionHatchMid);   break;
        case Arm::apHigh:   distanceOffset = Robot::m_dashboard.GetDashValue(dvVisionHatchHigh);  break;
        default:
          if (Robot::m_arm.GetHandModeSwitch() == Arm::hmHatch) {
            newPosition = Arm::apTravel;
            distanceOffset = Robot::m_dashboard.GetDashValue(dvVisionHatchLoad);
          } else {
            newPosition = Arm::apLoad;
            distanceOffset = Robot::m_dashboard.GetDashValue(dvVisionCargoLoad);
          }
      }

      Robot::m_vision.AddDistanceOffset(distanceOffset);
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
