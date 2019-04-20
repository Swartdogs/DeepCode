#include "commands/GrpSideHatchRight.h"
#include "Robot.h"

#include "commands/CmdArmSetArmPosition.h"
#include "commands/CmdArmSetHatchState.h"
#include "commands/CmdAutoStop.h"
#include "commands/CmdDriveDistance.h"
#include "commands/CmdDriveRotate.h"
#include "commands/CmdDriveToTarget.h"
#include "commands/CmdVisionFindTarget.h"

GrpSideHatchRight::GrpSideHatchRight(double distance1, double maxSpeed1) {
  AddParallel   (new CmdArmSetArmPosition(Arm::apTravel));
  AddSequential (new CmdDriveDistance(distance1, 0, maxSpeed1, 0, true, false, 4.0));
  AddSequential (new CmdDriveDistance(150, 20, 0.6, 0, true, false, 5.0));   
  AddSequential (new CmdDriveRotate(-90, 0.8, false, 2.5));
  AddSequential (new CmdVisionFindTarget(Vision::tsRight));
  AddSequential (new CmdDriveToTarget(0.5, 3.0, true, -34));
  AddSequential (new CmdArmSetHatchState(Arm::hsRelease));
  AddSequential (new CmdDriveDistance(-30, -90, 0.7, 0, true, false, 2.0));
  AddSequential (new CmdAutoStop(asHatchPlace));
  AddSequential (new CmdDriveRotate(-197, 0.8, false, 2.0));
  AddSequential (new CmdDriveDistance(168, -197, 0.8, 0, true, false, 4.5));
  AddSequential (new CmdDriveRotate(-185, 0.8, false, 1.5));
  AddSequential (new CmdAutoStop(asDriveToLoad));
  AddSequential (new CmdVisionFindTarget(Vision::tsBest));
  AddSequential (new CmdDriveToTarget(0.5, 6.0, true, Robot::m_dashboard.GetDashValue(dvVisionHatchLoad), true));
  AddSequential (new CmdAutoStop(asHatchLoad));
  AddSequential (new CmdDriveDistance(-90, -188, 0.7, 0, true, false, 3.5));
}

