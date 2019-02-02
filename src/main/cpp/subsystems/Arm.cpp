/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "subsystems/Arm.h"
#include "Robot.h"

Arm::Arm() : Subsystem("Arm") {
  m_armInUse = false;
  m_manualDrive = false;

  m_shoulderPosition = apTravel;
  m_wristPosition = apTravel;

  m_shoulderPot.SetAverageBits(2);
  m_shoulderPot.SetOversampleBits(0);
  m_wristPot.SetAverageBits(2);
  m_wristPot.SetOversampleBits(0);

  m_shoulderPID.SetCoefficient('P', 0, 0.04, 0);
  m_shoulderPID.SetCoefficient('I', 10, 0, 0.0025);
  m_shoulderPID.SetCoefficient('D', 0, 0.25, 0);
  m_shoulderPID.SetInputRange(-360, 360);
  m_shoulderPID.SetOutputRamp(0.25, 0.05);
  m_shoulderPID.SetSetpointDeadband(1.0); 

  m_wristPID.SetCoefficient('P', 0, 0.04, 0);
  m_wristPID.SetCoefficient('I', 10, 0, 0.0025);
  m_wristPID.SetCoefficient('D', 0, 0.25, 0);
  m_wristPID.SetInputRange(-360, 360);
  m_wristPID.SetOutputRamp(0.25, 0.05);
  m_wristPID.SetSetpointDeadband(1.0); 
}

void Arm::InitDefaultCommand() {
  // Set the default command for a subsystem here.
  // SetDefaultCommand(new MySpecialCommand());
}

void Arm::Periodic() {
  static bool wasManuallyDriven = false;
  double shoulderPower = 0;
  double wristPower = 0;

  if (!m_manualDrive) {
    if (wasManuallyDriven) {
      SetShoulderPosition(GetShoulderDegrees());
      SetWristPosition(GetWristDegrees());
    }

    shoulderPower = m_shoulderPID.Calculate(GetShoulderDegrees());
    wristPower = m_wristPID.Calculate(GetWristDegrees());
  
  } else {
    if(!wasManuallyDriven){
      m_shoulderPosition = apUnknown;
      m_wristPosition = apUnknown;
    }

    double joyX = Robot::m_oi.ApplyDeadband(Robot::m_oi.GetArmJoystickX(), 0.05);
    double joyY = Robot::m_oi.ApplyDeadband(Robot::m_oi.GetArmJoystickY(), 0.05);

    if ((GetShoulderDegrees() >= 7 && joyY > 0) || (GetShoulderDegrees() <= 4 && joyY < 0)) joyY = 0; //Update min and max values
    if ((GetWristDegrees() >= 7 && joyX > 0) || (GetWristDegrees() <= 4 && joyX < 0)) joyX = 0;

    shoulderPower = joyY;
    wristPower = joyX;
  }

  wasManuallyDriven = m_manualDrive;

  m_shoulderMotor.Set(shoulderPower);
  m_wristMotor.Set(wristPower);
}

bool Arm::GetArmInUse() {
  return m_armInUse;
}

std::string Arm::GetArmPositionName(ArmPosition position) {
  std::string name = "";

  switch (position) {
      case apTravel:    name = "Travel";    break;
      case apPickup:    name = "Pickup";    break;
      case apLow:       name = "Low";       break;
      case apMid:       name = "Mid";       break;
      case apHigh:      name = "High";      break;
      case apCargoShip: name = "CargoShip"; break;
      case apUnknown:   name = "Unknown";   break;
  }

  return name;
}

double Arm::GetShoulderDegrees() {
  return m_shoulderPot.GetAverageValue() / SHOULDER_COUNTS_PER_DEGREE; //TODO: Add offset
}

Arm::ArmPosition Arm::GetShoulderPosition(){
  return m_shoulderPosition;
}

double Arm::GetWristDegrees() {
  return m_wristPot.GetAverageValue() / WRIST_COUNTS_PER_DEGREE; //TODO: Add offset
}

Arm::ArmPosition Arm::GetWristPosition(){
  return m_wristPosition;
}

bool Arm::IsDrivenManually() {
  return m_manualDrive;
}

void Arm::SetArmInUse(bool inUse) {
  m_armInUse = inUse;
}

void Arm::SetDrivenManually(bool isManual) {
  m_manualDrive = isManual;
}

void Arm::SetShoulderPosition(ArmPosition position) {
  double degrees = 0;

  switch (position) {
      case apTravel:    degrees = 1;    break;
      case apPickup:    degrees = 2;    break;
      case apLow:       degrees = 3;    break;
      case apMid:       degrees = 4;    break;
      case apHigh:      degrees = 5;    break;
      case apCargoShip: degrees = 6;    break;
      default:                          return;
  }

  m_shoulderPosition = position;

  SetShoulderPosition(degrees, position);
}

void Arm::SetShoulderPosition(double degrees, ArmPosition position) {
  m_shoulderPID.SetSetpoint(degrees, GetShoulderPosition());
 
  if(position == apUnknown){
    sprintf(Robot::message,"Arm:      Set Shoulder Position=%4.1f", degrees);
  } else {
    sprintf(Robot::message,"Arm:      Set Shoulder Position=%s (%4.1f)", 
          GetArmPositionName(position).c_str(), degrees);
  }

  Robot::m_robotLog.Write(Robot::message);
}

void Arm::SetWristPosition(ArmPosition position) {
  double degrees = 0;

  switch (position) {
      case apTravel:    degrees = 1;    break;
      case apPickup:    degrees = 2;    break;
      case apLow:       degrees = 3;    break;
      case apMid:       degrees = 4;    break;
      case apHigh:      degrees = 5;    break;
      case apCargoShip: degrees = 6;    break;
      default:                          return;
  }

  m_wristPosition = position;

  SetWristPosition(degrees, position);
}

void Arm::SetWristPosition(double degrees, ArmPosition position) {
  m_wristPID.SetSetpoint(degrees, GetWristPosition());

  if(position == apUnknown){
    sprintf(Robot::message,"Arm:      Set Wrist Position=%4.1f", degrees);
  } else {
    sprintf(Robot::message,"Arm:      Set Wrist Position=%s (%4.1f)", 
          GetArmPositionName(position).c_str(), degrees);
  }

  Robot::m_robotLog.Write(Robot::message);
}

bool Arm::ShoulderAtSetpoint() {
  return m_shoulderPID.AtSetpoint();
}

bool Arm::WristAtSetpoint() {
  return m_wristPID.AtSetpoint();
}