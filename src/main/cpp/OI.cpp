#include "OI.h"
#include <frc/WPILib.h>
#include "commands/GrpClimb.h"
#include "commands/GrpEndHatch.h"
#include "commands/GrpEndHatchRight.h"
#include "commands/GrpEndHatchLeft.h"
#include "commands/GrpSideLeftHatch1.h"
#include "commands/GrpGoToTarget.h"
#include "commands/CmdArmIncrementShoulder.h"
#include "commands/CmdArmSetArmPosition.h"
#include "commands/CmdArmSetHatchState.h"
#include "commands/CmdArmSetSpeed.h"
#include "commands/CmdArmSetHandMode.h"
#include "commands/CmdArmSetIntakeMode.h"
#include "commands/CmdArmSetManual.h"
#include "commands/CmdArmSetPresetMode.h"
#include "commands/CmdCancelClimb.h"
#include "commands/CmdDriveSetGear.h"
#include "commands/CmdSandStormAuto.h"
#include "commands/CmdVisionFindTarget.h"
#include "commands/CmdVisionToggleMode.h"
#include "subsystems/Elevator.h"
#include "subsystems/Drive.h"
#include "subsystems/Vision.h"
#include "Robot.h"

OI::OI() {
  Robot::m_robotLog.Write("OI:       INIT", false);

  m_buttonDrive1.WhenPressed        (new GrpGoToTarget());
  m_buttonDrive2.WhenPressed        (new CmdDriveSetGear(Drive::spLow));
  m_buttonDrive2.WhenReleased       (new CmdDriveSetGear(Drive::spHigh));
  m_buttonDrive6.WhenPressed        (new CmdVisionToggleMode());
  m_buttonDrive7.WhenPressed        (new CmdVisionFindTarget(Vision::tsBest));
  m_buttonDrive8.WhenPressed        (new CmdSandStormAuto());
  m_buttonDrive10.WhenPressed       (new CmdCancelClimb());

  m_buttonArm1.WhenPressed          (new CmdArmSetManual(true));
  m_buttonArm1.WhenReleased         (new CmdArmSetManual(false));
  m_buttonArm2.WhenPressed          (new CmdArmSetArmPosition(Arm::apCargoCatch));
  m_buttonArm10.WhenPressed         (new CmdArmIncrementShoulder(-1.0));
  m_buttonArm11.WhenPressed         (new CmdArmIncrementShoulder(1.0));

  m_buttonBox1.WhenPressed          (new CmdArmSetArmPosition(Arm::apHigh));
  m_buttonBox5.WhenPressed          (new CmdArmSetArmPosition(Arm::apMid));
  m_buttonBox9.WhenPressed          (new CmdArmSetArmPosition(Arm::apLow));

  m_buttonBox2.WhenPressed          (new CmdArmSetArmPosition(Arm::apCargoShip));
  m_buttonBox6.WhenPressed          (new CmdArmSetArmPosition(Arm::apPickup));
  m_buttonBox10.WhenPressed         (new CmdArmSetArmPosition(Arm::apTravel));

  m_buttonBox3.WhenPressed          (new CmdArmSetHatchState(Arm::hsGrab));
  m_buttonBox7.WhenPressed          (new CmdArmSetHatchState(Arm::hsRelease));
  m_buttonBox11.WhenPressed         (new CmdArmSetPresetMode(true));
  m_buttonBox11.WhenReleased        (new CmdArmSetPresetMode(false));

  m_buttonBox4.WhenPressed          (new CmdArmSetIntakeMode(Arm::imOut));
  m_buttonBox8.WhenPressed          (new CmdArmSetSpeed(true));
  m_buttonBox8.WhenReleased         (new CmdArmSetSpeed(false));
  m_buttonBox12.WhenPressed         (new CmdArmSetIntakeMode(Arm::imIn));

  m_InternalLevel2.WhenPressed      (new GrpClimb(Elevator::epLevel2));
  m_InternalLevel3.WhenPressed      (new GrpClimb(Elevator::epLevel3));
  m_InternalHandMode.WhenPressed    (new CmdArmSetHandMode(Arm::hmHatch));
  m_InternalHandMode.WhenReleased   (new CmdArmSetHandMode(Arm::hmCargo));

  m_autoGroup = nullptr;
}

double OI::ApplyDeadband(double joystickValue, double deadband) {
  deadband = fabs(deadband);

  if (joystickValue > deadband) {
    return ((joystickValue - deadband) / (1 - deadband));
  } else if (joystickValue < -deadband) {
    return ((joystickValue + deadband) / (1 - deadband));
  }
  
  return 0;
}

bool OI::DriverCancel() {
  return (fabs(m_driveJoystick.GetY()) > 0.3 || fabs(m_driveJoystick.GetX()) > 0.3);
}

double OI::GetArmJoystickX() {
  return ApplyDeadband(m_armJoystick.GetX(), 0.15);
}

double OI::GetArmJoystickY() {
  return ApplyDeadband(m_armJoystick.GetY(), 0.15);
}

double OI::GetDriveJoystickX() {
  return ApplyDeadband(m_driveJoystick.GetX(), 0.05);
}

double OI::GetDriveJoystickY() {
  return -ApplyDeadband(m_driveJoystick.GetY(), 0.05);
}

bool OI::InHatchMode() {
  return (m_buttonBox.GetX() < -0.5);
}

void OI::Periodic() {
  m_InternalLevel2.SetPressed(m_buttonDrive8.Get() && m_buttonArm7.Get());
  m_InternalLevel3.SetPressed(m_buttonDrive8.Get() && m_buttonArm6.Get());
  m_InternalHandMode.SetPressed(m_buttonBox.GetX() < -0.5);
}

void OI::SandStormAutoInit() {
  switch ((int)Robot::m_dashboard.GetDashValue(dvAutoHatchPlace)) {
    case 1:   m_autoGroup = new GrpEndHatchLeft();    break; 
    case 2:   m_autoGroup = new GrpEndHatchRight();   break;
    case 3:   m_autoGroup = new GrpSideLeftHatch1();  break;
    default:  m_autoGroup = nullptr;
  }

  if (m_autoGroup != nullptr) m_autoGroup->Start();

}

bool OI::SandStormAutoRunning() {
  return false;

  if (m_autoGroup == nullptr) {
    return false;
  } else {
    return m_autoGroup->IsRunning();
  }
}
