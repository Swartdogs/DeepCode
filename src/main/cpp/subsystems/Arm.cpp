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

  m_handMode = hmCargo;

  m_hatchState = hsRelease;

  m_intakeMode = imOff;

  m_shoulderPosition = apUnknown;
  m_wristPosition = apUnknown;

  m_shoulderPot.SetAverageBits(2);
  m_shoulderPot.SetOversampleBits(0);
  m_wristPot.SetAverageBits(2);
  m_wristPot.SetOversampleBits(0);

  m_shoulderSetpoint = GetShoulderDegrees();
  m_wristSetpoint = GetWristDegrees();

  m_shoulderPID.SetCoefficient('P', 0, 0.04, 0);
  m_shoulderPID.SetCoefficient('I', 10, 0, 0.0025);
  m_shoulderPID.SetCoefficient('D', 0, 0.25, 0);
  m_shoulderPID.SetInputRange(-360, 360);
  m_shoulderPID.SetOutputRamp(0.25, 0.05);
  m_shoulderPID.SetSetpointDeadband(1.0); 
  m_shoulderPID.SetSetpoint(m_shoulderSetpoint, m_shoulderSetpoint);

  m_wristPID.SetCoefficient('P', 0, 0.04, 0);
  m_wristPID.SetCoefficient('I', 10, 0, 0.0025);
  m_wristPID.SetCoefficient('D', 0, 0.25, 0);
  m_wristPID.SetInputRange(-360, 360);
  m_wristPID.SetOutputRamp(0.25, 0.05);
  m_wristPID.SetSetpointDeadband(1.0); 
  m_wristPID.SetSetpoint(m_wristSetpoint, m_wristSetpoint);

  m_solHand.Set(false);
  m_solHatch.Set(false);
}

void Arm::InitDefaultCommand() {
  // Set the default command for a subsystem here.
  // SetDefaultCommand(new MySpecialCommand());
}

void Arm::Periodic() {
  static bool wasManuallyDriven = false;
  static int  timer           = 0;

  double      shoulderDegrees = GetShoulderDegrees();
  double      shoulderPower   = m_shoulderPID.Calculate(shoulderDegrees);;
  double      wristDegrees    = GetWristDegrees();
  double      wristPower      = m_wristPID.Calculate(wristDegrees);
  double      topPower        = 0;
  double      bottomPower     = 0;

  switch(m_intakeMode) {
    case imIn:
      if(m_cargoSensor.Get()) {
        topPower = Robot::m_dashboard.GetDashValue(dvCargoSpeedIn);
        bottomPower = topPower;
      } else {
        SetIntakeMode(imOff);        
      }

      break; 

    case imOut:
      if(timer < (int)(EJECT_TIMEOUT*1000)/50) {
        topPower = -Robot::m_dashboard.GetDashValue(dvCargoSpeedOut);
        bottomPower = topPower;
        
        if (m_cargoSensor.Get()) timer++;
        else timer = 0;

      } else {
        timer = 0;
        SetIntakeMode(imOff);        
      }
      break;

    case imRotate:
      topPower = Robot::m_dashboard.GetDashValue(dvCargoSpeedRotate);
      bottomPower = -topPower * Robot::m_dashboard.GetDashValue(dvCargoRotateRatio);
      break;

    default:;
  }

  if (m_manualDrive) {
    double joyShoulder  = Robot::m_oi.GetArmJoystickY();
    double joyWrist     = Robot::m_oi.GetArmJoystickX();

    if (joyShoulder < 0) {
      if (shoulderDegrees < Robot::m_dashboard.GetDashValue(dvShoulderMin) + 4) {
        shoulderPower = 0;
      } else {
        shoulderPower = joyShoulder;
      } 

      SetShoulderPosition(shoulderDegrees);
      
    } else if (joyShoulder > 0) {
      if ((shoulderDegrees < Robot::m_dashboard.GetDashValue(dvShoulderMax) - 4)
           && shoulderPower < joyShoulder) {
        shoulderPower = joyShoulder;
        SetShoulderPosition(shoulderDegrees);
      }
    } 
  }


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

    double joyX = Robot::m_oi.GetArmJoystickX();
    double joyY = Robot::m_oi.GetArmJoystickY();

    if ((GetShoulderDegrees() >= 7 && joyY > 0) || (GetShoulderDegrees() <= 4 && joyY < 0)) joyY = 0; //Update min and max values
    if ((GetWristDegrees() >= 7 && joyX > 0) || (GetWristDegrees() <= 4 && joyX < 0)) joyX = 0;
  }

  wasManuallyDriven = m_manualDrive;

  m_shoulderMotor.Set(shoulderPower);
  m_wristMotor.Set(wristPower);
  m_handTop.Set(topPower);
  m_handBottom.Set(bottomPower);
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

bool Arm::GetCargoDetected() {
  return !m_cargoSensor.Get();
}

Arm::HandMode Arm::GetHandMode() {
  return m_handMode;
}

std::string Arm::GetHandModeName(HandMode mode) {
 std::string name = "";

  switch (mode) {
      case hmCargo:    name = "Cargo";    break;
      case hmHatch:    name = "Hatch";    break;
  }

  return name; 
}

Arm::HatchState Arm::GetHatchState () {
  return m_hatchState;
}

std::string Arm::GetHatchStateName(HatchState state) {
   std::string name = "";

  switch (state) {
      case hsGrab:       name = "Grab";       break;
      case hsRelease:    name = "Release";    break;
  }

  return name; 
}

Arm::IntakeMode Arm::GetIntakeMode(){
  return m_intakeMode;
}

double Arm::GetShoulderDegrees() {
  return (m_shoulderPot.GetAverageValue() / SHOULDER_COUNTS_PER_DEGREE) + Robot::m_dashboard.GetDashValue(dvShoulderOffset); //TODO: Add offset
}

Arm::ArmPosition Arm::GetShoulderPosition(){
  return m_shoulderPosition;
}

double Arm::GetShoulderSetpoint() {
  return m_shoulderSetpoint;
}

double Arm::GetWristDegrees() {
  return (m_wristPot.GetAverageValue() / WRIST_COUNTS_PER_DEGREE) + Robot::m_dashboard.GetDashValue(dvWristOffset); //TODO: Add offset
}

Arm::ArmPosition Arm::GetWristPosition(){
  return m_wristPosition;
}

double Arm::GetWristSetpoint() {
  return m_wristSetpoint;
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

void Arm::SetHandMode(HandMode mode) {
  if(mode != m_handMode) {
    m_solHand.Set(mode == hmHatch);
    m_handMode = mode;
  }
}

void Arm::SetHatchState(HatchState state) {
  if (state != m_hatchState) {
    m_solHatch.Set(state == hsGrab);
    m_hatchState = state;
  }
}

void Arm::SetIntakeMode(IntakeMode mode) {
  if (mode != m_intakeMode) {
    m_intakeMode = mode;
  }
}

void Arm::SetShoulderPosition(ArmPosition position) {
  double degrees = 0;

  if(m_shoulderPosition != position) {
    switch (position) {
        case apTravel:    degrees = Robot::m_dashboard.GetDashValue(dvShoulderTravel);      break;
        case apPickup:    degrees = Robot::m_dashboard.GetDashValue(dvShoulderPickup);      break;
        case apLow:       degrees = Robot::m_dashboard.GetDashValue(dvShoulderRocketLow);   break;
        case apMid:       degrees = Robot::m_dashboard.GetDashValue(dvShoulderRocketMid);   break;
        case apHigh:      degrees = Robot::m_dashboard.GetDashValue(dvShoulderRocketHigh);  break;
        case apCargoShip: degrees = Robot::m_dashboard.GetDashValue(dvShoulderCargo);       break;
        default:                                                                            return;
    }

    SetShoulderPosition(degrees, position);
  }
}

void Arm::SetShoulderPosition(double degrees, ArmPosition position) {
  m_shoulderSetpoint = degrees;
  m_shoulderPosition = position;

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

  if (m_wristPosition != position) {
    switch (position) {
        case apTravel:    degrees = Robot::m_dashboard.GetDashValue(dvWristTravel);     break;
        case apPickup:    degrees = Robot::m_dashboard.GetDashValue(dvWristPickup);     break;
        case apLow:       degrees = Robot::m_dashboard.GetDashValue(dvWristRocketLow);  break;
        case apMid:       degrees = Robot::m_dashboard.GetDashValue(dvWristRocketMid);  break;
        case apHigh:      degrees = Robot::m_dashboard.GetDashValue(dvWristRocketHigh); break;
        case apCargoShip: degrees = Robot::m_dashboard.GetDashValue(dvWristCargo);      break;
        default:                                                                        return;
    }

    SetWristPosition(degrees, position);
  }
}

void Arm::SetWristPosition(double degrees, ArmPosition position) {
  m_wristSetpoint = degrees;
  m_wristPosition = position;
  
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