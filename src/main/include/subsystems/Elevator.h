#pragma once

#include "RobotMap.h"
#include <frc/WPILib.h>
#include "PIDControl.h"
#include <frc/commands/Subsystem.h>

class Elevator : public frc::Subsystem {
 public:
  static constexpr double   COUNTS_PER_INCH = 36.64;

  typedef enum {
    fpExtended,
    fpRetracted
  } FootPosition;

  typedef enum {
    fsFront,
    fsRear
  } FloorSensor;

  typedef enum {
    epRetracted,
    epLevel2,
    epLevel3,
    epMinHeight,
    epMaxHeight,
    epUnknown
  } ElevatorPosition;

  Elevator();

  void            InitDefaultCommand() override;
  void            Periodic() override;

  void            DriveFoot(double speed);
  bool            ElevatorAtSetpoint();
  bool            FloorDetected(FloorSensor sensor);
  bool            GetCancelClimb();
  double          GetElevatorPosition();
  std::string     GetElevatorPositionName(ElevatorPosition position) ;
  double          GetElevatorSetpoint();
  FootPosition    GetFootPosition();
  std::string     GetFootPositionName(FootPosition position);
  void            SetCancelClimb(bool cancelClimb);
  void            SetElevatorPosition(ElevatorPosition position);
  void            SetElevatorSetpoint(double setpoint);
  void            SetFootPosition(FootPosition position);
  void            SetFootInUse(bool inUse);

 private:
  FootPosition        m_footPosition;
  ElevatorPosition    m_elevatorPosition;
  bool                m_footInUse;
  bool                m_cancelClimb;
  double              m_elevatorSetpoint;

  PIDControl          m_elevatorPID{"Elevator"};

  frc::AnalogInput    m_elevatorPot{aioElevatorPot};

  frc::DigitalInput   m_frontSensor{dioFrontSensor};
  frc::DigitalInput   m_rearSensor{dioRearSensor};

  frc::Solenoid       m_footSol{solFoot};

  frc::VictorSP       m_elevatorMotor{pwmElevatorMotor};
  frc::VictorSP       m_footMotor{pwmFootMotor};
};
