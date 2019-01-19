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
    pwmDriveRight2
} RoboRioPwm;

typedef enum {
    dioEncoderLeftChannelA,
    dioEncoderLeftChannelB,
    dioEncoderRightChannelA,
    dioEncoderRightChannelB
} RoboRioDio;

typedef enum {
    solShifter
} RoboRioSol;

typedef enum {
    joyDrive
} DSJoystick;