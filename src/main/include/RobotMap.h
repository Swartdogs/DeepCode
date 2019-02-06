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
    dioSonarLeftEcho,
    dioSonarLeftPing,
    dioSonarRightEcho,
    dioSonarRightPing
} RoboRioDio;

typedef enum {
    solShifter
} RoboRioSol;
