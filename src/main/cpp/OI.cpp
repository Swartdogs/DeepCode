#include "OI.h"
#include <frc/WPILib.h>
#include "commands/GrpClimb.h"
#include "commands/CmdArmSetArmPosition.h"
#include "commands/CmdArmSetHandMode.h"
#include "commands/CmdArmSetIntakeMode.h"
#include "commands/CmdCancelClimb.h"
#include "commands/CmdDriveSetGear.h"
#include "commands/CmdArmSetHatchState.h"
#include "commands/CmdVisionFindTarget.h"
#include "subsystems/Elevator.h"
#include "subsystems/Drive.h"
#include "subsystems/Vision.h"

OI::OI() {
  m_InternalLevel2.WhenPressed      (new GrpClimb(Elevator::epLevel2));
  m_InternalLevel3.WhenPressed      (new GrpClimb(Elevator::epLevel3));

  m_buttonDrive10.WhenPressed       (new CmdCancelClimb());
  m_buttonDrive1.WhenPressed        (new CmdDriveSetGear(Drive::spLow));
  m_buttonDrive1.WhenReleased       (new CmdDriveSetGear(Drive::spHigh));

  m_buttonBox12.WhenPressed         (new CmdArmSetIntakeMode(Arm::imIn));
  m_buttonBox4.WhenPressed          (new CmdArmSetIntakeMode(Arm::imOut));
  m_buttonBox8.WhenPressed          (new CmdArmSetIntakeMode(Arm::imRotate));

  m_InternalHandMode.WhenPressed    (new CmdArmSetHandMode(Arm::hmHatch));
  m_InternalHandMode.WhenReleased   (new CmdArmSetHandMode(Arm::hmCargo));

  m_buttonBox3.WhenPressed          (new CmdArmSetHatchState(Arm::hsGrab));
  m_buttonBox7.WhenPressed          (new CmdArmSetHatchState(Arm::hsRelease));
  m_buttonBox11.WhenPressed         (new CmdArmSetArmPosition(Arm::apLoad));

  m_buttonBox1.WhenPressed          (new CmdArmSetArmPosition(Arm::apHigh));
  m_buttonBox5.WhenPressed          (new CmdArmSetArmPosition(Arm::apMid));
  m_buttonBox9.WhenPressed          (new CmdArmSetArmPosition(Arm::apLow));

  m_buttonBox2.WhenPressed          (new CmdArmSetArmPosition(Arm::apCargoShip));
  m_buttonBox6.WhenPressed          (new CmdArmSetArmPosition(Arm::apPickup));
  m_buttonBox10.WhenPressed         (new CmdArmSetArmPosition(Arm::apTravel));

  m_buttonArm1.WhenPressed          (new CmdVisionFindTarget(Vision::tsBest));
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

double OI::GetArmJoystickX() {
  //return ApplyDeadband(m_armJoystick.GetX(), 0.15);
  return 0;
}

double OI::GetArmJoystickY() {
  //return ApplyDeadband(m_armJoystick.GetY(), 0.15);
  return 0;
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
  m_InternalLevel2.SetPressed(m_buttonDrive9.Get() && m_buttonArm8.Get());
  m_InternalLevel3.SetPressed(m_buttonDrive9.Get() && m_buttonArm9.Get());
  m_InternalHandMode.SetPressed(m_buttonBox.GetX() < -0.5);
}