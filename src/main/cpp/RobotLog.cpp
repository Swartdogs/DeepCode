/*----------------------------------------------------------------------------*/
/* Copyright (c) 2018 FIRST. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <frc/WPILib.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <frc/Timer.h>

#include "RobotLog.h"
#include "Robot.h"

RobotLog::RobotLog(std::string name) {
    m_logFile          = nullptr;
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
    fclose(m_logFile);
    m_logFile = nullptr;

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
 //   m_periodicLastEnd = Timer::GetFPGATimestamp() *1000;

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
/*
    data = DataString(Robot::pdp.GetAmps(pdpDriveLeft1));
    */
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
        //sprintf(m_log, "%s: Periodic Usage=%5.1f %%", m_robotName.c_str(),
        //(m_periodicTotalTime / (Timer::GetFPGATimestamp() *1000 - m_periodicBeginTime)) *100);
        Write(m_log);
    }
}

void RobotLog::StartPeriodic() {

}

void RobotLog::Write(std::string entry, bool includeTime) {

}

void RobotLog::WriteData(std::string data){

}

void RobotLog::WritePid(std::string output) {

}