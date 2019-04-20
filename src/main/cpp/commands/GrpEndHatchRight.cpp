#include "commands/GrpEndHatchRight.h"
#include "Robot.h"

#include "commands/CmdArmSetArmPosition.h"
#include "commands/CmdArmSetHatchState.h"
#include "commands/CmdAutoStop.h"
#include "commands/CmdDriveDistance.h"
#include "commands/CmdDriveRotate.h"
#include "commands/CmdDriveToTarget.h"
#include "commands/CmdVisionFindTarget.h"

GrpEndHatchRight::GrpEndHatchRight() {
  AddParallel   (new CmdArmSetArmPosition(Arm::apTravel));
  AddSequential (new CmdDriveDistance(40, 0, 0.5, 0, true, false, 2.0));
  AddSequential (new CmdVisionFindTarget(Vision::tsRight));
  AddSequential (new CmdDriveToTarget(0.6, 4.5, true, Robot::m_dashboard.GetDashValue(dvVisionHatchLoad)));
  AddSequential (new CmdArmSetHatchState(Arm::hsRelease));
  AddSequential (new CmdDriveDistance(-18, 0, 0.7, 0, true, false, 2.0));
  AddSequential (new CmdAutoStop(asHatchPlace));
  AddSequential (new CmdDriveRotate(-60, 0.8, false, 2.5));
  AddSequential (new CmdDriveDistance(-120, -60, 0.8, 0, true, false, 4.0));
  AddSequential (new CmdDriveRotate(-180, 0.8, false, 2.5));
  AddSequential (new CmdAutoStop(asDriveToLoad));
  AddSequential (new CmdVisionFindTarget(Vision::tsBest));
  AddSequential (new CmdDriveToTarget(0.5, 6.0, true, Robot::m_dashboard.GetDashValue(dvVisionHatchLoad), true));
  AddSequential (new CmdAutoStop(asHatchLoad));
  AddSequential (new CmdDriveDistance(-90, -188, 0.7, 0, true, false, 3.5));
  AddSequential (new CmdDriveRotate(-330, 0.8, false, 3.5));
}
