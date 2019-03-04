#include "commands/GrpEndHatch.h"
#include "commands/CmdArmSetArmPosition.h"
#include "commands/CmdArmSetHandMode.h"
#include "commands/CmdArmSetHatchState.h"
#include "commands/CmdDriveDistance.h"
#include "commands/CmdDriveToTarget.h"
#include "commands/CmdVisionFindTarget.h"


GrpEndHatch::GrpEndHatch(Vision::TargetSelect target) {
  AddSequential (new CmdArmSetHandMode(Arm::hmHatch));
  AddSequential (new CmdDriveDistance(48, 0, 0.3, 0, true, true, 3.0));
  AddSequential (new CmdVisionFindTarget(target));
  AddSequential (new CmdArmSetArmPosition(Arm::apLow));
  AddSequential (new CmdDriveToTarget(0.3, 3.0));
  AddSequential (new frc::WaitCommand(0.5));
  AddSequential (new CmdArmSetHatchState(Arm::hsRelease));
  AddSequential (new CmdDriveDistance(-24, 0, 0.3, 0, true, true, 3.0));
  AddSequential (new CmdArmSetArmPosition(Arm::apTravel));
  AddSequential (new CmdArmSetHatchState(Arm::hsGrab));
}
