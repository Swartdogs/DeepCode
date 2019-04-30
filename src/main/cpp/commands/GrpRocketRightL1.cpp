#include "commands/GrpRocketRightL1.h"
#include "commands/CmdArmSetArmPosition.h"
#include "commands/CmdArmSetHatchState.h"
#include "commands/CmdArmSetPresetPosition.h"
#include "commands/CmdAutoStop.h"
#include "commands/CmdDriveDistance.h"
#include "commands/CmdDriveRotate.h"
#include "commands/CmdDriveToTarget.h"
#include "commands/CmdVisionFindTarget.h"

#include "Robot.h"

GrpRocketRightL1::GrpRocketRightL1() {
  AddParallel   (new CmdArmSetArmPosition(Arm::apTravel));
  AddSequential (new CmdDriveDistance(75, 0, 0.5, 0, true, false, 4.0));
  AddParallel   (new CmdArmSetPresetPosition(Arm::apLow));
  AddSequential (new CmdVisionFindTarget(Vision::tsBest));
  AddSequential (new CmdArmSetArmPosition(Arm::apPreset));
  AddSequential (new CmdDriveToTarget(0.4, 4.0, true));
  AddSequential (new CmdArmSetHatchState(Arm::hsRelease));
  AddSequential (new CmdDriveDistance(-16, 0, 0.7, 0, true, false, 2.0));
  AddSequential (new CmdArmSetArmPosition(Arm::apTravel));
  AddSequential (new CmdAutoStop(asHatchPlace));
  AddSequential (new CmdDriveRotate(130, 0.8, false, 2.0));
  AddSequential (new CmdDriveDistance(66, 130, 0.8, 0, true, false, 4.0));
  AddSequential (new CmdDriveRotate(145, 0.8, false, 2.0));
  AddSequential (new CmdVisionFindTarget(Vision::tsBest));
  AddSequential (new CmdAutoStop(asDriveToLoad));
  AddSequential (new CmdDriveToTarget(0.5, 6.0, true, Robot::m_dashboard.GetDashValue(dvVisionHatchLoad), true));
  AddSequential (new CmdAutoStop(asHatchLoad));
  AddSequential (new CmdDriveDistance(-90, 130, 0.7, 0, true, false, 3.5)); 
}