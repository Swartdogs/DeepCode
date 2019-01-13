/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "subsystems/Drive.h"

Drive::Drive() : Subsystem("Drive") {
  m_driveLeft1.SetInverted(true);
  m_driveLeft2.SetInverted(true);

  m_solShifter.Set(false);
  m_shifterPosition = spHigh;

  m_encoderLeft.SetDistancePerPulse(1.0);
  m_encoderRight.SetDistancePerPulse(1.0);

}

void Drive::InitDefaultCommand() {
  // Set the default command for a subsystem here.
  // SetDefaultCommand(new MySpecialCommand());
}

void Drive::ArcadeDrive(double drive, double rotate) {
  double left = drive + rotate;
  double right = drive - rotate;

  if (fabs(left) > 1.0 || fabs(right) > 1.0) {
    double max = std::max(fabs(left), fabs(right));

    left /= max;
    right /= max;
  }

  m_driveLeft1.Set(left);
  m_driveLeft2.Set(left);
  m_driveRight1.Set(right);
  m_driveRight2.Set(right);
}

void Drive::SetShifter(ShifterPosition position) {
  if (position != m_shifterPosition) {
    m_solShifter.Set(position == spLow);
    m_shifterPosition = position;
  }

}
