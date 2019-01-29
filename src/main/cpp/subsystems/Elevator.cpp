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
  m_footSol.Set(false);

  m_elevatorPID.SetCoefficient('P', 0, 0.04, 0);
  m_elevatorPID.SetCoefficient('I', 10, 0, 0.0025);
  m_elevatorPID.SetCoefficient('D', 0, 0.25, 0);
  m_elevatorPID.SetInputRange(-360, 360);
  m_elevatorPID.SetOutputRamp(0.25, 0.05);
  m_elevatorPID.SetSetpointDeadband(1.0); 
}

void Elevator::InitDefaultCommand() {
  // Set the default command for a subsystem here.
  // SetDefaultCommand(new MySpecialCommand());
}

void Elevator::DriveFoot(double speed) {
  if(GetFootPosition() == fpExtended || (GetPlatformStatus() != psOff && 
      GetElevatorPosition() < Robot::m_dashboard.GetDashValue(dvElevRetracted))) {
    m_footMotor.Set(speed > 1 ? 1 : (speed < -1 ? -1 : speed));
  }
}

bool Elevator::ElevatorAtSetpoint() {
  return m_elevatorPID.AtSetpoint();
}

void Elevator::Execute(double speed) {
  GetPlatformStatus();
  m_elevatorMotor.Set(m_elevatorPID.Calculate(GetElevatorPosition()));
  DriveFoot(speed);
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
    default:;
  }

  return name.c_str();
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

Elevator::PlatformStatus Elevator::GetPlatformStatus() {
  int lightSensorFlags;
  
  // Create a bitmap:
  //                    m_frontSensor m_rearSensor
  //                          v            v
  // lightSensorFlags = 0b    X            X 
  //
  // values for lightSensorFlags:
  //    0b00 = 0 = neither sensor sees the platform       (psOff)
  //    0b01 = 1 = m_rearSensor only sees the platform    (psUnknown)
  //    0b10 = 2 = m_frontSensor only sees the platform   (psPartial)
  //    0b11 = 3 = both sensors see the platform          (psOn)

  lightSensorFlags = (m_frontSensor.Get() ? 1 : 0) << 1 | (m_rearSensor.Get() ? 1 : 0);

  switch (lightSensorFlags) {
    case 0:
      m_platformStatus = psOff;
      break;

    case 2:
      m_platformStatus = psPartial;
      break;

    case 3:
      m_platformStatus = psOn;
      break;

    case 1:
    default:
      m_platformStatus = psUnknown;
      break;
  }

  return m_platformStatus;
}

void Elevator::SetElevatorPosition(ElevatorPosition position) {
  double setpoint = 0;

  switch (position) {
    case epRetracted:
      setpoint = Robot::m_dashboard.GetDashValue(dvElevRetracted);
      break;

    case epLevel2Extended:
      setpoint = Robot::m_dashboard.GetDashValue(dvElevLevel2);
      break;

    case epLevel3Extended:
      setpoint = Robot::m_dashboard.GetDashValue(dvElevLevel3);
      break;

    case epMinHeight:
      setpoint = Robot::m_dashboard.GetDashValue(dvElevMin);
      break;
    
    case epMaxHeight:
      setpoint = Robot::m_dashboard.GetDashValue(dvElevMax);
      break;

    default:;
  }
  
  SetElevatorSetpoint(setpoint);
}

void Elevator::SetElevatorSetpoint(double setpoint) {
  //setpoint *= COUNTS_PER_INCH;

  if (setpoint < Robot::m_dashboard.GetDashValue(dvElevMin)) setpoint = Robot::m_dashboard.GetDashValue(dvElevMin);
  if (setpoint > Robot::m_dashboard.GetDashValue(dvElevMax)) setpoint = Robot::m_dashboard.GetDashValue(dvElevMax);
  m_elevatorPID.SetSetpoint(setpoint, GetElevatorPosition());
}

void Elevator::SetFootPosition(FootPosition position) {
  if (position != m_footPosition) {
    m_footSol.Set(position == fpExtended);
    m_footPosition = position;
  }
}
