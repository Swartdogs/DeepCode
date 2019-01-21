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

  Elevator();

  void            InitDefaultCommand() override;
  void            DriveFoot(double speed);
  bool            ElevatorAtSetpoint();
  void            Execute();
  double          GetElevatorPosition();
  FootPosition    GetFootPosition();
  PlatformStatus  GetPlatformStatus();
  void            SetElevatorSetpoint(double setpoint);
  void            SetFootPosition(FootPosition position);

 private:
  const double        COUNTS_PER_INCH = 36.64;

  FootPosition        m_footPosition;
  PlatformStatus      m_platformStatus;

  PIDControl          m_elevatorPID{"Elevator"};

  frc::AnalogInput    m_elevatorPot{aioElevatorPot};

  frc::DigitalInput   m_frontSensor{dioFrontSensor};
  frc::DigitalInput   m_rearSensor{dioRearSensor};

  frc::Solenoid       m_footSol{solFoot};

  frc::VictorSP       m_elevatorMotor{pwmElevatorMotor};
  frc::VictorSP       m_footMotor{pwmFootMotor};
};
