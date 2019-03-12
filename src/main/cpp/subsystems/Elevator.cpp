#include "subsystems/Elevator.h"
#include "Robot.h"

Elevator::Elevator() : Subsystem("Elevator") {
  Robot::m_robotLog.Write("Elevator: INIT", false);

  m_elevatorPot.SetAverageBits(2);
  m_elevatorPot.SetOversampleBits(0);

  m_elevatorSetpoint = GetElevatorPosition();

  m_elevatorMotor.SetInverted(true);
  m_footMotor.SetInverted(true);

  m_elevatorPID.SetCoefficient('P', 0, 0.8, 0);
  m_elevatorPID.SetCoefficient('I', 1, 0, 0.06);
  m_elevatorPID.SetCoefficient('D', 0, 0, 0);
  m_elevatorPID.SetInputRange(-20, 1.3);
  m_elevatorPID.SetOutputRamp(0.10, 0.05);
  m_elevatorPID.SetSetpointDeadband(0.5); 
  m_elevatorPID.SetSetpoint(m_elevatorSetpoint, m_elevatorSetpoint);

  m_footSol.Set(false);

  m_cancelClimb = false;
  m_elevatorPosition = epUnknown;
  m_footPosition = fpExtended;
  m_footInUse = false;
}

void Elevator::InitDefaultCommand() {
}

void Elevator::Periodic() {
  m_elevatorMotor.Set(m_elevatorPID.Calculate(GetElevatorPosition()));
  if (!m_footInUse) m_footMotor.Set(0);
}

void Elevator::DriveFoot(double speed) {
  m_footMotor.Set(speed > 1.0 ? 1.0 : (speed < -1.0 ? -1.0 : speed));
}

bool Elevator::ElevatorAtSetpoint() {
  return m_elevatorPID.AtSetpoint();
}

bool Elevator::FloorDetected (FloorSensor sensor) {
  switch (sensor) {
    case fsFront: return  !m_frontSensor.Get();
    case fsRear:  return  !m_rearSensor.Get();
    default:      return  false;   
  }
}

bool Elevator::GetCancelClimb() {
  return m_cancelClimb;
}

double Elevator::GetElevatorPosition() {
  return (m_elevatorPot.GetAverageValue() * INCHES_PER_COUNT) - Robot::m_dashboard.GetDashValue(dvElevOffset);
}

std::string Elevator::GetElevatorPositionName(ElevatorPosition position)  {
  std::string name = "";

  switch (position) {
    case epRetracted:       name = "Retracted";       break;
    case epLevel2:          name = "Level2";          break;
    case epLevel3:          name = "Level3";          break;
    case epMinHeight:       name = "MinHeight";       break;
    case epMaxHeight:       name = "MaxHeight";       break;
    case epUnknown:         name = "Unknown";         break;
    default:;
  }

  return name;
}

double Elevator::GetElevatorSetpoint(){
  return m_elevatorSetpoint;
}

Elevator::FootPosition Elevator::GetFootPosition() {
  return m_footPosition;
}

std::string Elevator::GetFootPositionName(FootPosition position) {
  std::string name = "";

  switch (position) {
    case fpRetracted:       name = "Retracted";       break;
    case fpExtended:        name = "Extended";        break;
    default:;
  }

  return name;
}

bool Elevator::SafeToRaise() {
  if (!m_frontSensor.Get() && !m_rearSensor.Get()) {              // Both sensors see floor
    return true;
  } else if (m_frontSensor.Get() && m_rearSensor.Get()) {         // Both sensors don't see floor
    return true;
  }
  return false;
}

void Elevator::SetCancelClimb(bool cancelClimb) {
  m_cancelClimb = cancelClimb;
  if (m_cancelClimb) SetElevatorSetpoint(GetElevatorPosition());
}

void Elevator::SetElevatorMotor(double speed) {
  m_elevatorMotor.Set(speed);
}

void Elevator::SetElevatorPosition(ElevatorPosition position) {
  if (m_elevatorPosition != position) {
    m_elevatorPosition = position;

    switch (position) {
      case epRetracted:
        m_elevatorSetpoint = Robot::m_dashboard.GetDashValue(dvElevRetracted);
        break;

      case epLevel2:
        m_elevatorSetpoint = Robot::m_dashboard.GetDashValue(dvElevLevel2);
        break;

      case epLevel3:
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

    sprintf(Robot::message,"Elevator: Set Elevator Position=%s (%4.1f)", 
            GetElevatorPositionName(m_elevatorPosition).c_str(), m_elevatorSetpoint);
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

    sprintf(Robot::message,"Elevator: Set Height=%5.1f", setpoint);
    Robot::m_robotLog.Write(Robot::message);
  }
}

void Elevator::SetFootPosition(FootPosition position) {
  if (position != m_footPosition) {
    m_footSol.Set(position == fpRetracted);
    m_footPosition = position;
    Robot::m_dashboard.SetRobotStatus(rsFootRetracted, position == fpRetracted);

    sprintf(Robot::message,"Elevator: Set Foot Position=%s", GetFootPositionName(m_footPosition).c_str());
    Robot::m_robotLog.Write(Robot::message);
  }
}

void Elevator::SetFootInUse(bool inUse) {
  m_footInUse = inUse;
}
