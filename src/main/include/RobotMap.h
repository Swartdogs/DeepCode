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
    pwmElevatorMotor,
    pwmFootMotor,
    pwmShoulder,
    pwmWrist
} RoboRioPwm;
    
typedef enum {    
    canDriveLeft1,
    canDriveLeft2,
    canDriveRight1,
    canDriveRight2
} RoboRioCan;

typedef enum {
    dioEncoderLeftChannelA,
    dioEncoderLeftChannelB,
    dioEncoderRightChannelA,
    dioEncoderRightChannelB,
    dioFrontSensor,
    dioRearSensor,
    dioSonarLeftEcho,
    dioSonarLeftPing,
    dioSonarRightEcho,
    dioSonarRightPing
} RoboRioDio;

typedef enum {
    aioElevatorPot,
    aioShoulderPot,
    aioWristPot
} RoboRioAio;

typedef enum {
    solShifter,
    solFoot
} RoboRioSol;

typedef enum {
    rsNone
} RobotStatus;

typedef enum {
    rvDriveGyro,
    rvDriveEncoderL,
    rvDriveEncoderR,
    rvDriveAmpsLeft1,
    rvDriveAmpsLeft2,
    rvDriveAmpsRight1,
    rvDriveAmpsRight2,
    rvElevatorPosition,
    rvElevatorSetpoint,
    rvElevatorFoot,
    rvElevatorAmps,
    rvShoulderPosition,
    rvShoulderSetpoint,
    rvShoulderAmps,
    rvWristPosition,
    rvWristSetpoint,
    rvWristAmps
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
    dvDriveRatio,
    dvShoulderMin, 
    dvShoulderMax,
    dvShoulderOffset,
    dvShoulderTravel,
    dvShoulderFloor,
    dvShoulderCargo,
    dvShoulderRocketLow,
    dvShoulderRocketMid,
    dvShoulderRocketHigh,
    dvShoulderModify,
    dvWristMin,
    dvWristMax,
    dvWristOffset,
    dvWristTravel,
    dvWristFloor,
    dvWristCargo,
    dvWristRocketLow,
    dvWristRocketMid,
    dvWristRocketHigh,
    dvWristModify
} DashValue;
