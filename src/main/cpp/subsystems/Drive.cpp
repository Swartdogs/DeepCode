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

  m_rotatePID.SetCoefficient('P', 0, 0.04, 0);
  m_rotatePID.SetCoefficient('I', 10, 0, 0.0025);
  m_rotatePID.SetCoefficient('D', 0, 0.25, 0);
  m_rotatePID.SetInputRange(-360, 360);
  m_rotatePID.SetOutputRamp(0.25, 0.05);
  m_rotatePID.SetSetpointDeadband(1.0); 

  m_drivePID.SetCoefficient('P', 0, 0.04, 0);
  m_drivePID.SetCoefficient('I', 10, 0, 0.0025);
  m_drivePID.SetCoefficient('D', 0, 0.25, 0);
  m_drivePID.SetInputRange(-500, 500);
  m_drivePID.SetOutputRamp(0.25, 0.05);
  m_drivePID.SetSetpointDeadband(1.0); 

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

double Drive::RotateExec() {
  return m_rotatePID.Calculate(m_gyro.GetAngle());

}

void Drive::RotateInit(double heading, double maxSpeed, bool resetGyro) {
  maxSpeed = fabs(maxSpeed); 

  if ( resetGyro ) m_gyro.Reset();
  m_rotatePID.SetSetpoint(heading, m_gyro.GetAngle());
  m_rotatePID.SetOutputRange(-maxSpeed, maxSpeed);
  
}

bool Drive::RotateIsFinished() {
  return m_rotatePID.AtSetpoint();
  
}

double Drive::DriveExec() {
  return m_drivePID.Calculate();
}

void Drive::DriveInit(double distance, double heading, double maxSpeed, bool resetEncoder, bool resetGyro){

}

bool Drive::DriveIsFinished(){

}

double Drive::GetDistance(UseEncoder encoder){
  double distance = 0
  double left = m_encoderLeft.GetDistance();
  double right = m_encoderRight.GetDistance();
}