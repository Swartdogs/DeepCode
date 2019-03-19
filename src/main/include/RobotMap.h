#pragma once

typedef enum {              // Command Status   (Commands)
    csRun,
    csSkip,
    csDone,
    csCancel,
    csTimedOut, 
    csContinue
} CommandStatus;

typedef enum {              // Robot Mode       (Dashboard, RobotLog)
    rmInit,					
	rmDisabled,
    rmAutonomous,
    rmTeleop,
    rmTest
} RobotMode;

typedef enum {              // PWM channels on RoboRIO
    pwmElevatorMotor,
    pwmFootMotor,
    pwmShoulder,
    pwmWrist,
    pwmHandTop,
    pwmHandBottom
} RoboRioPwm;
    
typedef enum {              // CAN Ids for Victors
    canUnused,
    canDriveLeft1,
    canDriveLeft2,
    canDriveRight1,
    canDriveRight2
} RoboRioCan;

typedef enum {              // Digital Input/Output channels on RoboRIO
    dioUnused0,
    dioUnused1,
    dioUnused2,
    dioCargoSensor,
    dioFrontSensor,
    dioRearSensor,
    dioEncoderLeftChannelA,
    dioEncoderLeftChannelB,
    dioEncoderRightChannelA,
    dioEncoderRightChannelB,
} RoboRioDio;

typedef enum {              // Analog channels on RoboRIO
    aioElevatorPot,
    aioShoulderPot,
    aioWristPot
} RoboRioAio;

typedef enum {              // Solenoid channels on PCM
    solShifter,
    solFoot,
    solHandCargo,
    solHatch,
    solHandHatch
} RoboRioSol;

typedef enum {              // Power channels on PDP
    pdpElevator     = 3,
    pdpShoulder     = 8,
    pdpWrist        = 9,
    pdpDriveLeft2   = 12,
    pdpDriveLeft1   = 13,
    pdpDriveRight2  = 14,
    pdpDriveRight1  = 15,
} PdpPowerChannels;

typedef enum {              // Dashboard: Robot Status indexes
    rsShifterLow,
    rsFootRetracted,
    rsFloorFront,
    rsFloorRear,
    rsCargo,
    rsHatchMode,
    rsHatchGrab,
    rsClimb,
    rsNoClimb,
    rsTargetFound, 
    rsCargoLoaded
} RobotStatus;

typedef enum {              // Dashboard: Robot Value indexes
    rvDriveGyro,
    rvDriveEncoderL,
    rvDriveEncoderR,
    rvDriveAmpsLeft1,
    rvDriveAmpsLeft2,
    rvDriveAmpsRight1,
    rvDriveAmpsRight2,
    rvElevatorPosition,
    rvElevatorSetpoint,
    rvElevatorAmps,
    rvShoulderPosition,
    rvShoulderSetpoint,
    rvShoulderAmps,
    rvWristPosition,
    rvWristSetpoint,
    rvWristAmps,
    rvVisionStatus,
    rvVisionSelect,
    rvVisionAngle,
    rvVisionDistance
} RobotValue;

typedef enum {              // Dashborad: Dash Button indexes
    dbRunPid,
    dbDataCapture,
    dbVisionAuto
} DashButton;

typedef enum{               // Dashboard: Dash Value indexes
    dvAutoSelect,
    dvAutoDelay,
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
    dvFloorSensorMin,
    dvElevRetracted,
    dvElevLevel2,
    dvElevLevel3,
    dvShoulderOffset,
    dvShoulderMin, 
    dvShoulderMax,
    dvShoulderClear,
    dvShoulderTravel,
    dvSCPickup,
    dvSCLoad,
    dvSCCargoShip,
    dvSCRocketLow,
    dvSCRocketMid,
    dvSCRocketHigh,
    dvSCCatch,
    dvSHRocketLow,
    dvSHRocketMid,
    dvSHRocketHigh,
    dvWristOffset,
    dvWristMin,
    dvWristMax,
    dvWristClear,
    dvWristTravel,
    dvWCPickup,
    dvWCLoad,
    dvWCCargoShip,
    dvWCRocketLow,
    dvWCRocketMid,
    dvWCRocketHigh,
    dvWCCatch,
    dvWHRocketLow,
    dvWHRocketMid,
    dvWHRocketHigh,
    dvCargoSpeedIn,
    dvCargoSpeedOut,
    dvCargoSpeedRotate,
    dvCargoRotateRatio,
    dvCargoEjectTime,
    dvCargoRotateTime,
    dvVisionAngleOffset,
    dvVisionCargoOffset,
    dvVisionHatchOffset
} DashValue;
