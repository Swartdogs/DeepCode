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
    dioEncoderLeftChannelA,
    dioEncoderLeftChannelB,
    dioEncoderRightChannelA,
    dioEncoderRightChannelB,
    dioFrontSensor,
    dioRearSensor,
    dioSonarLeftEcho,
    dioSonarLeftPing,
    dioSonarRightEcho,
    dioSonarRightPing,
    dioCargoSensor
} RoboRioDio;

typedef enum {              // Analog channels on RoboRIO
    aioElevatorPot,
    aioShoulderPot,
    aioWristPot
} RoboRioAio;

typedef enum {              // Solenoid channels on PCM
    solShifter,
    solFoot,
    solHand,
    solHatch
} RoboRioSol;

typedef enum {              // Power channels on PDP
    pdpElevator,
    pdpShoulder,
    pdpWrist,
    pdpDriveLeft2   = 12,
    pdpDriveLeft1   = 13,
    pdpDriveRight2  = 14,
    pdpDriveRight1  = 15,
} PdpPowerChannels;

typedef enum {              // Dashboard: Robot Status indexes
    rsNone
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
    rvElevatorFoot,
    rvElevatorAmps,
    rvShoulderPosition,
    rvShoulderSetpoint,
    rvShoulderAmps,
    rvWristPosition,
    rvWristSetpoint,
    rvWristAmps
} RobotValue;

typedef enum {              // Dashborad: Dash Button indexes
    dbRunPid
} DashButton;

typedef enum{               // Dashboard: Dash Value indexes
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
    dvShoulderPickup,
    dvShoulderCargo,
    dvShoulderRocketLow,
    dvShoulderRocketMid,
    dvShoulderRocketHigh,
    dvShoulderModify,
    dvWristMin,
    dvWristMax,
    dvWristOffset,
    dvWristTravel,
    dvWristPickup,
    dvWristCargo,
    dvWristRocketLow,
    dvWristRocketMid,
    dvWristRocketHigh,
    dvWristModify,
    dvIntakeSpeed,
    dvIntakeRatio
} DashValue;
