#include "Robot.h"
#include "subsystems/Drive.h"
#include "subsystems/Vision.h"
#include "commands/GrpLineUp.h"
#include "commands/CmdVisionTakeImage.h"
#include "commands/CmdDriveRotate.h"
#include "commands/CmdDriveDistance.h"

GrpLineUp::GrpLineUp() {
  AddSequential (new CmdVisionTakeImage(Vision::tsBest));
  AddSequential (new CmdDriveRotate(Robot::m_vision.GetTargetAngle(), 0.7, true, 2.0, false));
  AddSequential (new CmdDriveDistance(((Drive::SONAR_THRESHOLD > Robot::m_vision.GetTargetDistance()) ? 0 : (Robot::m_vision.GetTargetDistance() - Drive::SONAR_THRESHOLD)), 
                 Robot::m_vision.GetTargetAngle(), 0.5, 0.2, true, false, 6.0, false, false));
  AddSequential (new CmdDriveDistance(6, Robot::m_vision.GetTargetAngle(), 0.5, 0.2, false, false, 4.0, true, false));
  AddSequential (new CmdDriveRotate(0, 0.7, true, 2.0, true));
}
