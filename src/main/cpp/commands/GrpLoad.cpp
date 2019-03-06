#include "commands/GrpLoad.h"
#include "Robot.h"
#include "commands/CmdArmSetArmPosition.h"
#include "commands/CmdArmSetHandMode.h"
#include "commands/CmdArmSetHatchState.h"
#include "commands/CmdDriveDistance.h"
#include "commands/CmdDriveToTarget.h"
#include "commands/CmdVisionFindTarget.h"

GrpLoad::GrpLoad() {
  AddSequential (new CmdVisionFindTarget());
  AddSequential (new CmdArmSetArmPosition(Arm::apByHand));
  AddSequential (new CmdDriveToTarget(0.4, 3.0));
}

void GrpLoad::End() {
  Robot::m_dashboard.SetRobotStatus(rsTargetFound, false);
}

void GrpLoad::Interrupted() {
  End();
}
