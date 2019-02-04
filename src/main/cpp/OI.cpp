#include "OI.h"
#include <frc/WPILib.h>

OI::OI() {

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
  return m_armJoystick.GetX();
}

double OI::GetArmJoystickY() {
  return -m_armJoystick.GetY();
}
