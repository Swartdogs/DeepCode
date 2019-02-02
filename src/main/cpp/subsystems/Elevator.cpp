#include "subsystems/Elevator.h"
#include "Robot.h"

Elevator::Elevator() : Subsystem("Elevator") {
  m_elevatorPot.SetAverageBits(2);
  m_elevatorPot.SetOversampleBits(0);

  m_elevatorPID.SetCoefficient('P', 0, 0.04, 0);
  m_elevatorPID.SetCoefficient('I', 10, 0, 0.0025);
  m_elevatorPID.SetCoefficient('D', 0, 0.25, 0);
  m_elevatorPID.SetInputRange(-360, 360);
  m_elevatorPID.SetOutputRamp(0.25, 0.05);
  m_elevatorPID.SetSetpointDeadband(1.0); 
  m_elevatorPID.SetSetpoint(m_elevatorSetpoint, m_elevatorSetpoint);

  m_footSol.Set(false);

  m_cancelClimb = false;
  m_elevatorPosition = epUnknown;
  m_elevatorSetpoint = GetElevatorPosition();
  m_footPosition = fpExtended;
  m_footInUse = false;

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

bool Elevator::FloorDetected (FloorSensor sensor) {
  if (GetElevatorPosition() < Robot::m_dashboard.GetDashValue(dvSensorMin)) return false;

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
  return (m_elevatorPot.GetAverageValue() / COUNTS_PER_INCH) - Robot::m_dashboard.GetDashValue(dvElevOffset);
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
    if (GetElevatorPosition() < Robot::m_dashboard.GetDashValue(dvSensorMin)) return true;

    return (!m_frontSensor.Get() && !m_rearSensor.Get());
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

    sprintf(Robot::message,"Elevator: Set Foot Position=%s", GetFootPositionName(m_footPosition).c_str());
    Robot::m_robotLog.Write(Robot::message);
  }
}

void Elevator::SetFootInUse(bool inUse) {
  m_footInUse = inUse;
}
