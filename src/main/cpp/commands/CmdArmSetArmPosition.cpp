// Sets the Arm Shoulder and Wrist setpoints to a specified Arm Position
//   Optionally waits for the Shoulder and Arm to reach those setpoints

#include "commands/CmdArmSetArmPosition.h"
#include "Robot.h"

CmdArmSetArmPosition::CmdArmSetArmPosition(Arm::ArmPosition position, bool waitForDone) {
  m_doneDelay   = 0;
  m_handDelay   = 0;
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

    m_doneDelay = 0;
    m_handDelay = 0;
    Arm::ArmPosition newPosition = m_position;
    
    if (newPosition == Arm::apPreset) {                                       // Use Preset Position
      newPosition = Robot::m_arm.GetPresetPosition();                         // Get Preset Position
      double targetDistance = Robot::m_vision.GetTargetDistance();
      double distanceOffset = 0;                                                

      switch (newPosition) {
        case Arm::apLow:    distanceOffset = Robot::m_dashboard.GetDashValue(dvVisionHatchLow);
                            if (targetDistance < 80) m_doneDelay = 15;
                            break;
        case Arm::apMid:    distanceOffset = Robot::m_dashboard.GetDashValue(dvVisionHatchMid);   
                            if (targetDistance < 100) m_doneDelay = 25;
                            break;
        case Arm::apHigh:   distanceOffset = Robot::m_dashboard.GetDashValue(dvVisionHatchHigh);  
                            if (targetDistance < 120) m_doneDelay = 50;
                            break;
        default:
          if (Robot::m_arm.GetHandModeSwitch() == Arm::hmHatch) {
            newPosition = Arm::apTravel;
            distanceOffset = Robot::m_dashboard.GetDashValue(dvVisionHatchLoad);

          } else {
            newPosition = Arm::apLoad;
            if (targetDistance < 100) m_doneDelay = 25;
            distanceOffset = Robot::m_dashboard.GetDashValue(dvVisionCargoLoad);
          }
      }

      if (m_doneDelay > 0) m_status = csRun;
      Robot::m_vision.AddDistanceOffset(distanceOffset);
    
    } else if (!Robot::m_arm.InPresetMode()) {
      if (Robot::m_arm.GetShoulderPosition() == Arm::apTravel &&
          Robot::m_arm.GetHandModeSwitch() == Arm::hmHatch) {
        switch (newPosition) {
          case Arm::apLow:
          case Arm::apMid:
          case Arm::apHigh:
            m_handDelay = 4;                          // Delay to allow Hand to move before Arm movement
            Robot::m_arm.SetHandModeRobot(Arm::hmHatch);
            m_status = csRun;
            return;
        }
      }
    }

    Robot::m_arm.SetArmPosition(newPosition);
  }
}

void CmdArmSetArmPosition::Execute() {
  if (m_status == csRun) {    
    if (m_handDelay > 0) {                          // Hand Delay
      m_handDelay--;
      if (m_handDelay <= 0) {                       // Hand Delay expired
        Robot::m_arm.SetArmPosition(m_position);    // Move Arm to new position
        if (!m_waitForDone) m_status = csDone;
      }

    } else if (m_doneDelay > 0) {                   // Delay while Arm moves
      m_doneDelay--;
      if (m_doneDelay <= 0) {                       // Done Delay expired
        if (!m_waitForDone) m_status = csDone;
      }

    } else if (Robot::m_arm.ShoulderAtSetpoint() && Robot::m_arm.WristAtSetpoint()) {
      m_status = csDone;                            // Done if Shoulder and Wrist at setpoints
    }
  }
}

bool CmdArmSetArmPosition::IsFinished() { 
  return (m_status != csRun); 
}

void CmdArmSetArmPosition::End() {}

void CmdArmSetArmPosition::Interrupted() {}
