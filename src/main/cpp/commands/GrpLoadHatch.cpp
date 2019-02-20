#include "commands/GrpLoadHatch.h"
#include "commands/CmdArmSetArmPosition.h"
#include "commands/CmdArmSetHandMode.h"
#include "commands/CmdArmSetHatchState.h"
#include "commands/CmdDriveDistance.h"
#include "commands/CmdDriveToTarget.h"
#include "commands/CmdVisionFindTarget.h"

GrpLoadHatch::GrpLoadHatch() {
  AddSequential (new CmdVisionFindTarget());
//  AddSequential (new CmdArmSetHatchState(Arm::hsRelease));
  AddSequential (new CmdArmSetArmPosition(Arm::apLow));
  AddSequential (new CmdDriveToTarget(-70.0, 0.4, 3.0));
}
