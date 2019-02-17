#include "OI.h"
#include <frc/WPILib.h>
#include "commands/GrpClimb.h"
#include "commands/CmdArmSetArmPosition.h"
#include "commands/CmdArmSetHandMode.h"
#include "commands/CmdArmSetIntakeMode.h"
#include "commands/CmdCancelClimb.h"
#include "commands/CmdDriveSetGear.h"
#include "commands/CmdArmSetHatchState.h"
#include "subsystems/Elevator.h"
#include "subsystems/Drive.h"

OI::OI() {
  //m_InternalLevel2.WhenPressed    (new GrpClimb(Elevator::epLevel2));
  //m_InternalLevel3.WhenPressed    (new GrpClimb(Elevator::epLevel3));
  m_buttonClimbEnable.WhenPressed   (new GrpClimb(Elevator::epLevel2));
  m_buttonClimbCancel.WhenPressed   (new CmdCancelClimb());
  m_buttonShifter.WhenPressed       (new CmdDriveSetGear(Drive::spLow));
  m_buttonShifter.WhenReleased      (new CmdDriveSetGear(Drive::spHigh));

  m_button12.WhenPressed            (new CmdArmSetIntakeMode(Arm::imIn));
  m_button4.WhenPressed             (new CmdArmSetIntakeMode(Arm::imOut));
  m_button8.WhenPressed             (new CmdArmSetIntakeMode(Arm::imRotate));

  m_InternalHandMode.WhenPressed    (new CmdArmSetHandMode(Arm::hmHatch));
  m_InternalHandMode.WhenReleased   (new CmdArmSetHandMode(Arm::hmCargo));

  m_button3.WhenPressed             (new CmdArmSetHatchState(Arm::hsGrab));
  m_button7.WhenPressed             (new CmdArmSetHatchState(Arm::hsRelease));

  m_button1.WhenPressed             (new CmdArmSetArmPosition(Arm::apHigh));
  m_button5.WhenPressed             (new CmdArmSetArmPosition(Arm::apMid));
  m_button9.WhenPressed             (new CmdArmSetArmPosition(Arm::apLow));

  m_button2.WhenPressed             (new CmdArmSetArmPosition(Arm::apCargoShip));
  m_button6.WhenPressed             (new CmdArmSetArmPosition(Arm::apPickup));
  m_button10.WhenPressed            (new CmdArmSetArmPosition(Arm::apTravel));

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
  //m_InternalLevel2.SetPressed(m_buttonClimbEnable.Get() && m_buttonClimbLevel2.Get());
  //m_InternalLevel3.SetPressed(m_buttonClimbEnable.Get() && m_buttonClimbLevel3.Get());
  m_InternalHandMode.SetPressed(m_buttonBox.GetX() < -0.5);
}