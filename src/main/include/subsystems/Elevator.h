/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

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
    psOff,
    psPartial,
    psOn,
    psUnknown
  } PlatformStatus;

  typedef enum {
    epRetracted,
    epLevel2Extended,
    epLevel3Extended,
    epMinHeight,
    epMaxHeight
  } ElevatorPosition;

  Elevator();

  void            InitDefaultCommand() override;
  void            Periodic() override;

  void            DriveFoot(double speed);
  bool            ElevatorAtSetpoint();
  double          GetElevatorPosition();
  const char*     GetElevatorPositionName(ElevatorPosition position);
  double          GetElevatorSetpoint();
  FootPosition    GetFootPosition();
  const char*     GetFootPositionName(FootPosition position);
  PlatformStatus  GetPlatformStatus();
  void            SetElevatorPosition(ElevatorPosition position);
  void            SetElevatorSetpoint(double setpoint);
  void            SetFootPosition(FootPosition position);
  void            SetFootInUse(bool inUse);

 private:
  FootPosition        m_footPosition;
  bool                m_footInUse;
  PlatformStatus      m_platformStatus;
  double              m_elevatorSetpoint;

  PIDControl          m_elevatorPID{"Elevator"};

  frc::AnalogInput    m_elevatorPot{aioElevatorPot};

  frc::DigitalInput   m_frontSensor{dioFrontSensor};
  frc::DigitalInput   m_rearSensor{dioRearSensor};

  frc::Solenoid       m_footSol{solFoot};

  frc::VictorSP       m_elevatorMotor{pwmElevatorMotor};
  frc::VictorSP       m_footMotor{pwmFootMotor};
};
