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
    pwmDriveLeft1,
    pwmDriveLeft2,
    pwmDriveRight1,
    pwmDriveRight2
} RoboRioPwm;

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
