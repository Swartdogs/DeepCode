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
    dvElevLevel3,
    dvDriveRatio
} DashValue;
