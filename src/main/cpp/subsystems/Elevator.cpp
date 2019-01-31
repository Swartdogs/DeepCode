/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "subsystems/Elevator.h"
#include "Robot.h"

Elevator::Elevator() : Subsystem("Elevator") {
  m_footPosition = fpRetracted;
  m_elevatorPosition = epUnknown;
  m_footInUse = false;
  //m_footSol.Set(false);
  m_elevatorPot.SetAverageBits(2);
  m_elevatorPot.SetOversampleBits(0);
  m_elevatorSetpoint = GetElevatorPosition();
  m_cancelClimb = false;

  m_elevatorPID.SetCoefficient('P', 0, 0.04, 0);
  m_elevatorPID.SetCoefficient('I', 10, 0, 0.0025);
  m_elevatorPID.SetCoefficient('D', 0, 0.25, 0);
  m_elevatorPID.SetInputRange(-360, 360);
  m_elevatorPID.SetOutputRamp(0.25, 0.05);
  m_elevatorPID.SetSetpointDeadband(1.0); 
  m_elevatorPID.SetSetpoint(m_elevatorSetpoint, m_elevatorSetpoint);
}

void Elevator::InitDefaultCommand() {
  // Set the default command for a subsystem here.
  // SetDefaultCommand(new MySpecialCommand());
}

void Elevator::Periodic() {
  m_elevatorMotor.Set(m_elevatorPID.Calculate(GetElevatorPosition()));

  if (!m_footInUse) m_footMotor.Set(0);
}

void Elevator::DriveFoot(double speed) {
  m_footMotor.Set(speed > 1 ? 1 : (speed < -1 ? -1 : speed));
}

bool Elevator::ElevatorAtSetpoint() {
  return m_elevatorPID.AtSetpoint();
}

bool Elevator::GetCancelClimb() {
  return m_cancelClimb;
}

double Elevator::GetElevatorPosition() {
  return (m_elevatorPot.GetAverageValue() / COUNTS_PER_INCH) - Robot::m_dashboard.GetDashValue(dvElevOffset);
}

const char* Elevator::GetElevatorPositionName(ElevatorPosition position) {
  std::string name = "";

  switch (position) {
    case epRetracted:       name = "Retracted";       break;
    case epLevel2Extended:  name = "Level2Extended";  break;
    case epLevel3Extended:  name = "Level3Extended";  break;
    case epMinHeight:       name = "MinHeight";       break;
    case epMaxHeight:       name = "MaxHeight";       break;
    case epUnknown:         name = "Unknown";         break;
    default:;
  }

  return name.c_str();
}

double Elevator::GetElevatorSetpoint(){
  return m_elevatorSetpoint;
}

Elevator::FootPosition Elevator::GetFootPosition() {
  return m_footPosition;
}

const char* Elevator::GetFootPositionName(FootPosition position) {
  std::string name = "";

  switch (position) {
    case fpRetracted:       name = "Retracted";       break;
    case fpExtended:        name = "Extended";        break;
    default:;
  }

  return name.c_str();
}

// Elevator::PlatformStatus Elevator::GetPlatformStatus() {
// int lightSensorFlags;

// // Create a bitmap:
// //                    m_frontSensor m_rearSensor
// //                          v            v
// // lightSensorFlags = 0b    X            X 
// //
// // values for lightSensorFlags:
// //    0b00 = 0 = neither sensor sees the platform       (psOff)
// //    0b01 = 1 = m_rearSensor only sees the platform    (psUnknown)
// //    0b10 = 2 = m_frontSensor only sees the platform   (psPartial)
// //    0b11 = 3 = both sensors see the platform          (psOn)

// lightSensorFlags = (m_frontSensor.Get() ? 1 : 0) << 1 | (m_rearSensor.Get() ? 1 : 0);

// switch (lightSensorFlags) {
//   case 0:
//     m_platformStatus = psOff;
//     break;

//   case 2:
//     m_platformStatus = psPartial;
//     break;

//   case 3:
//     m_platformStatus = psOn;
//     break;

//   case 1:
//   default:
//     m_platformStatus = psUnknown;
//     break;
// }

// return m_platformStatus;
// }

bool Elevator::FloorDetected (FloorSensor sensor) {
  if (GetElevatorPosition() < Robot::m_dashboard.GetDashValue(dvSensorMin)) return false;

  switch (sensor) {
    case fsFront: return  !m_frontSensor.Get();
    case fsRear:  return  !m_rearSensor.Get();
    default:      return  false;   
  }
}

void Elevator::SetCancelClimb(bool cancelClimb) {
  m_cancelClimb = cancelClimb;
}

void Elevator::SetElevatorPosition(ElevatorPosition position) {
  if (m_elevatorPosition != position) {
    m_elevatorPosition = position;

    switch (position) {
      case epRetracted:
        m_elevatorSetpoint = Robot::m_dashboard.GetDashValue(dvElevRetracted);
        break;

      case epLevel2Extended:
        m_elevatorSetpoint = Robot::m_dashboard.GetDashValue(dvElevLevel2);
        break;

      case epLevel3Extended:
        m_elevatorSetpoint = Robot::m_dashboard.GetDashValue(dvElevLevel3);
        break;

      case epMinHeight:
        m_elevatorSetpoint = Robot::m_dashboard.GetDashValue(dvElevMin);
        break;
      
      case epMaxHeight:
        m_elevatorSetpoint = Robot::m_dashboard.GetDashValue(dvElevMax);
        break;

      default:;
    }
    
    m_elevatorPID.SetSetpoint(m_elevatorSetpoint, GetElevatorPosition());

    sprintf(Robot::message,"Elevator:    Set Elevator Position=%s (%4.1f)", 
            GetElevatorPositionName(m_elevatorPosition), m_elevatorSetpoint);

    Robot::m_robotLog.Write(Robot::message);
  }
}

void Elevator::SetElevatorSetpoint(double setpoint) {
  if (m_elevatorSetpoint != setpoint) {
    m_elevatorPosition = epUnknown;

    if (setpoint < Robot::m_dashboard.GetDashValue(dvElevMin)) setpoint = Robot::m_dashboard.GetDashValue(dvElevMin);
    if (setpoint > Robot::m_dashboard.GetDashValue(dvElevMax)) setpoint = Robot::m_dashboard.GetDashValue(dvElevMax);
    
    m_elevatorSetpoint = setpoint;
    m_elevatorPID.SetSetpoint(m_elevatorSetpoint, GetElevatorPosition());

    sprintf(Robot::message,"Elevator:    SetHeight   Position=%5.1f", setpoint);

    Robot::m_robotLog.Write(Robot::message);
  }
}

void Elevator::SetFootPosition(FootPosition position) {
  if (position != m_footPosition) {
    //m_footSol.Set(position == fpExtended);
    m_footPosition = position;

    sprintf(Robot::message,"Elevator:    SetFoot   Position=%s", GetFootPositionName(m_footPosition));

    Robot::m_robotLog.Write(Robot::message);
  }
}

void Elevator::SetFootInUse(bool inUse) {
  m_footInUse = inUse;
}
