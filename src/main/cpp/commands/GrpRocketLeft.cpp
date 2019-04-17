#include "commands/GrpRocketLeft.h"
#include "commands/CmdVisionFindTarget.h"
#include "commands/CmdDriveDistance.h"
#include "commands/CmdDriveToTarget.h"
#include "commands/CmdArmSetHatchState.h"

#include "Robot.h"

GrpRocketLeft::GrpRocketLeft() {
  AddSequential (new CmdDriveDistance(40, -35, 0.6, 0, true, false, 4.0));
  AddSequential (new CmdVisionFindTarget(Vision::tsBest));
  AddSequential (new CmdDriveToTarget(0.5, 4.0, true, -40));
  AddSequential (new CmdArmSetHatchState(Arm::hsGrab));
  AddSequential (new CmdDriveDistance(-30, 0, 0.7, 0, true, true, 2.0));
  
}
