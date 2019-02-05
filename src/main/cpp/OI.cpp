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

double OI::GetDriveJoystickX(){
  return m_driveJoystick.GetX();
}

double OI::GetDriveJoystickY(){
  return -m_driveJoystick.GetY();
}
