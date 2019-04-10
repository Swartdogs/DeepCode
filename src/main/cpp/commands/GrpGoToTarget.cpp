#include "commands/GrpGoToTarget.h"
#include "Robot.h"
#include "commands/CmdArmSetArmPosition.h"
#include "commands/CmdDriveToTarget.h"
#include "commands/CmdVisionFindTarget.h"

GrpGoToTarget::GrpGoToTarget() {
  AddSequential (new CmdVisionFindTarget());
  AddSequential (new CmdArmSetArmPosition(Arm::apPreset));
  AddSequential (new CmdDriveToTarget(0.4, 3.5));
}

void GrpGoToTarget::End() {
  Robot::m_dashboard.SetRobotStatus(rsTargetFound, false);
}

void GrpGoToTarget::Interrupted() {
  End();
}
