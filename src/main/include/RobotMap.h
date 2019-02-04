#pragma once

typedef enum {
    rmInit,					// Robot Mode 		(Dashboard, RobotLog)
	rmDisabled,
    rmAutonomous,
    rmTeleop,
    rmTest
} RobotMode;

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
