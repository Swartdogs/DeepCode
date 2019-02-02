/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "subsystems/Drive.h"
#include "commands/CmdDriveJoystick.h"

Drive::Drive() : Subsystem("Drive") {
  m_driveEnable = true;
  m_driveInUse  = false;

  m_driveRight1.SetInverted(true);
  m_driveRight2.SetInverted(true);

  m_solShifter.Set(false);
  m_shifterPosition = spHigh;

  m_useEncoder = ueLeftEncoder;

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

Drive::~Drive() {}

void Drive::InitDefaultCommand() {
  // Set the default command for a subsystem here.
  SetDefaultCommand(new CmdDriveJoystick());
}

void Drive::Periodic() {
  if (!m_driveInUse) {
    ArcadeDrive(0, 0);
  }
}

void Drive::ArcadeDrive(double drive, double rotate) {
  double left = 0;
  double right = 0;

  if (m_driveEnable) {

    left = drive + rotate;
    right = drive - rotate;

    if (fabs(left) > 1.0 || fabs(right) > 1.0) {
      double max = std::max(fabs(left), fabs(right));

      left /= max;
      right /= max;
    }
  }

  m_driveLeft1.Set(left);
  m_driveLeft2.Set(left);
  m_driveRight1.Set(right);
  m_driveRight2.Set(right);
}

double Drive::DriveExec() {
  return m_drivePID.Calculate(GetDistance(m_useEncoder));
}

void Drive::DriveInit(double distance, double heading, double maxSpeed, double minSpeed,
                      bool resetEncoder, bool resetGyro, bool driveArc) {
  maxSpeed = fabs(maxSpeed);
  minSpeed = fabs(minSpeed); 

  double headingNow = 0; 
  if (!resetGyro) headingNow = GetHeading();

  if (fabs(heading - headingNow) < 5) {
    m_useEncoder = ueBothEncoders;
  } else if (heading > headingNow) {
    m_useEncoder = ueLeftEncoder; 
  } else {
    m_useEncoder = ueRightEncoder;
  }

  if (resetEncoder) {
    m_encoderLeft.Reset();
    m_encoderRight.Reset();
  }

  m_drivePID.SetSetpoint(distance, GetDistance(m_useEncoder));
  m_drivePID.SetOutputRange(-maxSpeed, maxSpeed, minSpeed); 

  driveArc ? RotateInit(headingNow, 0.7, resetGyro) :
             RotateInit(heading, 0.7, resetGyro);
}

bool Drive::DriveIsFinished(){
  return m_drivePID.AtSetpoint();
}

void Drive::DrivePidTune() {
  m_encoderLeft.Reset();
  m_encoderRight.Reset();

  m_useEncoder = ueLeftEncoder;
}

double Drive::GetDistance(UseEncoder encoder){
  double distance = 0;
  double left = m_encoderLeft.GetDistance();
  double right = m_encoderRight.GetDistance();
  UseEncoder source = encoder;

  if (source == ueCurrentEncoder) source = m_useEncoder;

  switch (source) {
    case ueCurrentEncoder:

    case ueLeftEncoder: 
      distance = left;
      break;
      
    case ueRightEncoder:
      distance = right;
      break;

    case ueBothEncoders:
      if (left < (right * 0.9)) distance = right;
      else if (right < (left * 0.9)) distance = left;
      else distance = (left + right) / 2;
      break;

    default:;
  }
  return distance;
}

double Drive::GetHeading() {
  return m_gyro.GetAngle ();
}

Drive::ShifterPosition Drive::GetShifterPosition () { 
  return m_shifterPosition; 
}

double Drive::RotateExec() {
  return m_rotatePID.Calculate(GetHeading());
}

void Drive::RotateInit(double heading, double maxSpeed, bool resetGyro) {
  maxSpeed = fabs(maxSpeed); 

  if ( resetGyro ) m_gyro.Reset();
  m_rotatePID.SetSetpoint(heading, GetHeading());
  m_rotatePID.SetOutputRange(-maxSpeed, maxSpeed);
}

bool Drive::RotateIsFinished() {
  return m_rotatePID.AtSetpoint();
}

void Drive::RotatePidTune() {
  m_gyro.Reset();
}

void Drive::SetDriveEnable(bool enable) {
  m_driveEnable = enable; 
}

void Drive::SetDriveInUse(bool inUse) {
  m_driveInUse = inUse;
}

void Drive::SetShifter(ShifterPosition position) {
  if (position != m_shifterPosition) {
    m_solShifter.Set(position == spLow);
    m_shifterPosition = position;
  }
}