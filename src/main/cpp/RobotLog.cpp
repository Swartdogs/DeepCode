#include <frc/WPILib.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <sys/time.h>

#include "RobotLog.h"
#include "Robot.h"

RobotLog::RobotLog(std::string name) {
    m_logFile           = nullptr;
    m_dataFile          = nullptr;
    m_pidFile           = nullptr;

    m_periodicBeginTime = 0;
    m_periodicCount     = 0;
    m_periodicLastEnd   = 0;
    m_periodicLastStart = 0;
    m_periodicTotalTime = 0;
    m_robotMode         = rmInit;
    m_robotName         = name;

    Write("", false);
    Write("*************** START 525 CODE ***************", false);
}

RobotLog::~RobotLog(){
}

void RobotLog::Close() {
    if (m_logFile != nullptr)
    {
        fclose(m_logFile);
        m_logFile = nullptr;
    }

    if (m_dataFile != nullptr) {
        fclose(m_dataFile);
        m_dataFile = nullptr;
    }

    if (m_pidFile != nullptr){
        fclose(m_pidFile);
        m_pidFile = nullptr;
    }
}

std::string RobotLog::DataString(double value) {

    std::stringstream ss;
    ss << std::fixed << std::setprecision(6) << value;

    std::string ds= ss.str();
    ds.erase(ds.find_last_not_of('0') + 1, std::string::npos);

    std::string::iterator it = ds.end() - 1;
    if (*it == '.') ds.erase(it);
    
    return ds + "|";
}

void RobotLog::EndPeriodic() {
    m_periodicCount++;
    m_periodicLastEnd = (double)frc::RobotController::GetFPGATime() / 1000;

    double runTime = m_periodicLastEnd - m_periodicLastStart;

    m_periodicTotalTime += runTime;

    if(runTime > 14.0) {
        sprintf(m_log, "%s: Long Periodic Execution Time=%5.1f (Start-to-End)", m_robotName.c_str(), runTime);
        Write(m_log);
    }
}

RobotMode RobotLog::GetMode() {
    return m_robotMode;
}

void RobotLog::LogData() {
    std::string data;

    data = DataString(Robot::m_drive.GetHeading());
    data += DataString(Robot::m_drive.GetDistance(Drive::ueLeftEncoder));
    data += DataString(Robot::m_drive.GetDistance(Drive::ueRightEncoder));
    data += DataString((double) Robot::m_drive.GetShifterPosition());

    data += DataString(Robot::m_arm.GetShoulderDegrees());
    data += DataString(Robot::m_arm.GetShoulderSetpoint());
    data += DataString(Robot::m_arm.GetWristDegrees());
    data += DataString(Robot::m_arm.GetWristSetpoint());
}

std::string RobotLog::ModeName(RobotMode mode) {
    switch (mode) {
        case rmInit:        return "INIT";
        case rmDisabled:    return "DISABLED";
        case rmAutonomous:  return "AUTO";
        case rmTeleop:      return "TELEOP";
        case rmTest:        return "TEST";
        default:            return "?";
    }
}

void RobotLog::SetMode(RobotMode mode) {
    m_periodicCount = 0;

    if(m_robotMode == rmAutonomous || m_robotMode == rmTeleop) {
        sprintf(m_log, "%s: Periodic CPU Usage=%5.1f %%", m_robotName.c_str(),
               (m_periodicTotalTime / ((double)frc::RobotController::GetFPGATime() / 1000 - m_periodicBeginTime)) * 100);
        Write(m_log, false);
    }
    m_robotMode = mode;
    m_periodicLastStart = (double) frc::RobotController::GetFPGATime() / 1000;
    m_periodicLastEnd = m_periodicLastStart;
    m_periodicBeginTime = m_periodicLastStart;
    m_periodicTotalTime = 0;

    Write("", false);
    sprintf(m_log, "%s: %s", m_robotName.c_str(), ModeName(mode).c_str());
    Write(m_log, false);
}

void RobotLog::StartPeriodic() {
    static bool inBrownOut = false;
    double timeNow = frc::RobotController::GetFPGATime() / 1000;

    if((timeNow - m_periodicLastStart) > 100) {
        sprintf(m_log, "%s: Long Periodic Interval=%5.1f (Start-to-Start)", m_robotName.c_str(), timeNow - m_periodicLastStart);
        Write(m_log);
    }
    m_periodicLastStart = timeNow;

    bool brownOut = frc::RobotController::IsBrownedOut();

    if (inBrownOut != brownOut) {
        inBrownOut = brownOut;
        if (inBrownOut) {
            sprintf(m_log, "%s: System Brown Out", m_robotName.c_str());
            Write(m_log);
        }
    }
}

void RobotLog::Write(std::string entry, bool includeTime, bool forceClose) {
    if (m_logFile == nullptr) m_logFile = fopen("/home/lvuser/Log525.txt", "a");
 
    if (entry.length() == 0) {
        printf("\n");
        if (m_logFile != nullptr) fprintf(m_logFile, "\r\n");

    } else { 
        const char* cEntry = entry.c_str();
        char        timeNow[20];
        struct      timeval tv;

        gettimeofday(&tv, NULL);
        int         millisec = (int)round(tv.tv_usec / 1000);
        std::time_t tod = tv.tv_sec;
        std::strftime(timeNow, 20, "%D %T.", std::localtime(&tod));

        if (includeTime) {
            float eventTime = (float)(m_periodicCount * 20) / 1000;
            int timeDiff = m_periodicCount - (int)((((double) frc::RobotController::GetFPGATime() / 1000)- m_periodicBeginTime) / 20);

            printf("%s%03d  %7.2f %+3d: %s \n", timeNow, millisec, eventTime, timeDiff, cEntry);
            if (m_logFile != nullptr) fprintf(m_logFile, "%s%03d  %7.2f %+3d: %s \r\n", timeNow, millisec, eventTime, timeDiff, cEntry);

        } else {
            printf("%s%03d  %s \n", timeNow, millisec, cEntry);        
            if (m_logFile != nullptr) fprintf(m_logFile, "%s%03d  %s \r\n", timeNow, millisec, cEntry);
        }
    }

    if (forceClose){
        if (m_logFile != nullptr) {
            fclose(m_logFile);
            m_logFile = nullptr;
        }
    }
}

void RobotLog::WriteData(std::string data){
    const char* cData = data.c_str();
    float eventTime = (float)(m_periodicCount * 20) / 1000;

    if (m_dataFile == nullptr) {
        m_dataFile = fopen("/home/lvuser/Data525.txt", "a");
        fprintf(m_dataFile, "%s\r\n", ModeName(m_robotMode).c_str());
    }

    if (m_dataFile != nullptr) fprintf(m_dataFile, "%7.2f|%s\r\n", eventTime, cData);
}

void RobotLog::WritePid(std::string output) {
    const char* pid = output.c_str();
    float eventTime = (float)(m_periodicCount * 20) / 1000;

    printf("%7.2f: %s \n", eventTime, pid);

    if (m_pidFile == nullptr) {
        m_pidFile = fopen("/home/lvuser/Pid525.txt", "a");
        fprintf(m_pidFile, "%s\r\n", ModeName(m_robotMode).c_str());
    }

    if (m_pidFile != nullptr) fprintf(m_pidFile, "%7.2f: %s \r\n", eventTime, pid);
}