/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#pragma once

typedef enum {
    csRun,
    csSkip,
    csDone,
    csCancel,
    csTimedOut, 
    csContinue
} CommandStatus;

typedef enum {
    rmInit,					// Robot Mode 		(Dashboard, RobotLog)
	rmDisabled,
    rmAutonomous,
    rmTeleop,
    rmTest
} RobotMode;

typedef enum {
    pwmDriveLeft1,
    pwmDriveLeft2,
    pwmDriveRight1,
    pwmDriveRight2,
    pwmElevatorMotor,
    pwmFootMotor
} RoboRioPwm;

typedef enum {
    dioEncoderLeftChannelA,
    dioEncoderLeftChannelB,
    dioEncoderRightChannelA,
    dioEncoderRightChannelB,
    dioFrontSensor,
    dioRearSensor
} RoboRioDio;

typedef enum {
    aioElevatorPot
} RoboRioAio;

typedef enum {
    solShifter,
    solFoot
} RoboRioSol;

typedef enum {
    rsNone
} RobotStatus;

typedef enum {
    rvNone
} RobotValue;

typedef enum { 
    dbRunPid
} DashButton;

typedef enum{ 
    dvPidSelect,
    dvPidSetpoint,
    dvPidMaxOut,
    dvPthreshold,
    dvPabove,
    dvPbelow,
    dvIthreshold,
    dvIabove,
    dvIbelow,
    dvDthreshold,
    dvDabove,
    dvDbelow,
    dvElevOffset,
    dvElevMin,
    dvElevMax,
    dvSensorMin,
    dvElevRetracted,
    dvElevLevel2,
    dvElevLevel3
} DashValue;


// const double elevatorRetractedSetpoint = 0;
// const double elevatorLevel2Setpoint = elevatorRetractedSetpoint - 10;
// const double elevatorLevel3Setpoint = elevatorRetractedSetpoint - 20;
// const double elevatorMinHeight = elevatorRetractedSetpoint - 10;
// const double elevatorMaxHeight = elevatorRetractedSetpoint + 90;
