#include <frc/WPILib.h>
#include <fstream>
#include <sstream>
#include <iomanip>

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

void RobotLog::EndPeriodic(){
    m_periodicCount++;
    m_periodicLastEnd = (double)frc::RobotController::GetFPGATime() / 1000;

    double runTime = m_periodicLastEnd - m_periodicLastStart;

    m_periodicTotalTime += runTime;

    if(runTime > 10.0) {
        sprintf(m_log, "%s: Long Periodic Execution Time=%5.1f (Start-to-End)", m_robotName.c_str(), runTime);
        Write(m_log);
    }
}

RobotMode RobotLog::GetMode() {
    return m_robotMode;
}

void RobotLog::LogData() {
    std::string data;

//  data = DataString(Robot::pdp.GetAmps(pdpDriveLeft1));
}

std::string RobotLog::ModeName(RobotMode mode) {
    switch (mode) {
        case rmInit:        return "Init";
        case rmDisabled:    return "Disabled";
        case rmAutonomous:  return "Auto";
        case rmTeleop:      return "Teleop";
        case rmTest:        return "Test";
        default:            return "?";
    }
}

void RobotLog::SetMode(RobotMode mode) {
    m_periodicCount = 0;

    if(m_robotMode == rmAutonomous || m_robotMode == rmTeleop) {
        sprintf(m_log, "%s: Periodic Usage=%5.1f %%", m_robotName.c_str(),
               (m_periodicTotalTime / ((double)frc::RobotController::GetFPGATime() / 1000 - m_periodicBeginTime)) * 100);
        Write(m_log, false);
    }
    m_robotMode = mode;
    m_periodicLastStart = (double) frc::RobotController::GetFPGATime() / 1000;
    m_periodicLastEnd = m_periodicLastStart;
    m_periodicBeginTime = m_periodicLastStart;
    m_periodicTotalTime = 0;
    
    Write("", false);
    //sprintf(m_log, "%s: Start %s %s", m_robotName.c_str(), ModeName(mode).c_str(), Robot::dash.GetTimeStamp().c_str());
    sprintf(m_log, "%s: Start %s", m_robotName.c_str(), ModeName(mode).c_str());
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

void RobotLog::Write(std::string entry, bool includeTime) {
    const char* cEntry=entry.c_str();
    float eventTime = (float)(m_periodicCount * 20) / 1000;
    int timeDiff = m_periodicCount - (int)((((double) frc::RobotController::GetFPGATime() / 1000)- m_periodicBeginTime) / 20);

    includeTime ? printf("%7.2f %+3d: %s \n", eventTime, timeDiff, cEntry) : printf("%s \n", cEntry);

    if (m_logFile == nullptr) m_logFile = fopen("/home/lvuser/Log525.txt", "a");

    if (m_logFile != nullptr) {
        (m_periodicCount > 0) ? fprintf(m_logFile, "%7.2f %+3d: %s \r\n", eventTime, timeDiff, cEntry) : fprintf(m_logFile, "%s \r\n", cEntry);
    }
}

void RobotLog::WriteData(std::string data){
    const char* cData = data.c_str();
    float eventTime = (float)(m_periodicCount * 20) / 1000;

    if (m_dataFile == nullptr) {
        m_dataFile = fopen("/home/lvuser/Data525.txt", "a");
        //fprintf(m_dataFile, "%s|%s\r\n", ModeName(m_robotMode).c_str(), Robot::dash.GetTimeStamp().c_str());
    }

    if (m_dataFile != nullptr) fprintf(m_dataFile, "%7.2f|%s\r\n", eventTime, cData);
}

void RobotLog::WritePid(std::string output) {
    const char* pid = output.c_str();
    float eventTime = (float)(m_periodicCount * 20) / 1000;

    printf("%7.2f: %s \n", eventTime, pid);

    if (m_pidFile == nullptr) {
        m_pidFile = fopen("/home/lvuser/Pid525.txt", "a");
        //fprintf(m_pidFile, "%s %s \r\n", ModeName(m_robotMode).c_str(), Robot::dash.GetTimeStamp().c_str());
    }

    if (m_pidFile != nullptr) fprintf(m_pidFile, "%7.2f: %s \r\n", eventTime, pid);
}