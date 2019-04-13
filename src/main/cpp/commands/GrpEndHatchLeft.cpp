#include "commands/GrpEndHatchRight.h"
#include "commands/CmdVisionFindTarget.h"
#include "commands/CmdDriveToTarget.h"
#include "commands/CmdArmSetHatchState.h"
#include "commands/CmdDriveDistance.h"
#include "commands/CmdDriveRotate.h"
#include "commands/CmdArmSetArmPosition.h"
#include "commands/CmdAutoHatchLoad.h"
#include "Robot.h"

#include "commands/GrpEndHatchLeft.h"

GrpEndHatchLeft::GrpEndHatchLeft() {
  AddSequential (new CmdVisionFindTarget(Vision::tsLeft));
  AddSequential (new CmdDriveToTarget(0.6, 5.0, true, Robot::m_dashboard.GetDashValue(dvVisionHatchLoad)));
  AddSequential (new CmdArmSetHatchState(Arm::hsRelease));
  AddSequential (new CmdDriveDistance(-18, 0, 0.6, 0, true, true, 3.0));
  AddSequential (new CmdDriveRotate(60, 0.8, false, 3.0));
  AddSequential (new CmdDriveDistance(-110, 60, 0.8, 0, true, false, 6.0));
  AddSequential (new CmdDriveRotate(180, 0.8, false, 3.0));
  AddSequential (new CmdAutoHatchLoad());
  AddSequential (new CmdVisionFindTarget(Vision::tsBest));
  AddSequential (new CmdDriveToTarget(0.45, 5.0, true, Robot::m_dashboard.GetDashValue(dvVisionHatchLoad)));
  AddSequential (new frc::WaitCommand(0.8));
  AddSequential (new CmdArmSetHatchState(Arm::hsGrab));
  AddSequential (new CmdDriveDistance(-90, 8, 0.7, 0, true, true, 3.0));
  AddSequential (new CmdDriveRotate(150, 0.8, true, 3.0));
} 
