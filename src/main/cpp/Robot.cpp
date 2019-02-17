#include "Robot.h"

#include <frc/commands/Scheduler.h>

char      Robot::message[100];

RobotLog  Robot::m_robotLog("Yeti");
Dashboard Robot::m_dashboard("2019", 1, 20, 1, 47);       //Dashboard and Log should be created first
Arm       Robot::m_arm; 
Drive     Robot::m_drive;
Vision    Robot::m_vision;
Elevator  Robot::m_elevator;
OI        Robot::m_oi;

void Robot::RobotInit() { 
 // m_vision.InitVision();
  StartDriverCamera();
}

void Robot::RobotPeriodic() {
  m_oi.Periodic();
  SetDashRobotValues();
}

void Robot::DisabledInit() {
  m_robotLog.SetMode(rmDisabled);
  m_robotLog.Close(); 
  m_dashboard.SetRobotMode(rmDisabled);

  m_arm.SetShoulderPosition(m_arm.GetShoulderDegrees());
  m_arm.SetWristPosition(m_arm.GetWristDegrees());
  m_elevator.SetElevatorPosition(Elevator::epRetracted);
  m_elevator.SetFootPosition(Elevator::fpExtended);
}

void Robot::DisabledPeriodic() { 
  frc::Scheduler::GetInstance()->Run(); 
  }

void Robot::AutonomousInit() {
  m_robotLog.SetMode(rmAutonomous);
  m_dashboard.SetRobotMode(rmAutonomous);
  m_arm.SetHandMode(m_arm.GetHandMode(), true);
  m_arm.SetShoulderPosition(m_arm.GetShoulderDegrees());
  m_arm.SetWristPosition(m_arm.GetWristDegrees());
  // if (m_oi.InHatchMode()) {
  //   m_arm.SetHandMode(Arm::hmHatch);
  // } else {
  //   m_arm.SetHandMode(Arm::hmCargo);
  // }
}

void Robot::AutonomousPeriodic() { 
  frc::Scheduler::GetInstance()->Run(); 
}

void Robot::TeleopInit() {
  m_robotLog.SetMode(rmTeleop);
  m_dashboard.SetRobotMode(rmTeleop);
  m_arm.SetHandMode(m_arm.GetHandMode(), true);
  m_arm.SetShoulderPosition(m_arm.GetShoulderDegrees());
  m_arm.SetWristPosition(m_arm.GetWristDegrees());

  // if (m_oi.InHatchMode()) {
  //   m_arm.SetHandMode(Arm::hmHatch);
  // } else {
  //   m_arm.SetHandMode(Arm::hmCargo);
  // }
}

void Robot::TeleopPeriodic() { 
  m_robotLog.StartPeriodic();
  frc::Scheduler::GetInstance()->Run(); 
  m_robotLog.EndPeriodic();
}

void Robot::TestInit() {
  m_robotLog.SetMode(rmTest);
  m_dashboard.SetRobotMode(rmTest);
}

void Robot::TestPeriodic() {
  static double       setpoint = 0;
  static int          tunePID = -1;
  
  if(m_dashboard.GetDashButton(dbRunPid)) {
      if(tunePID < 0) { 
        m_robotLog.WritePid("");
        tunePID   = m_dashboard.GetDashValue(dvPidSelect);
        setpoint  = m_dashboard.GetDashValue(dvPidSetpoint);

        switch(tunePID) {
            case 0:                       // Rotate PID
              m_robotLog.WritePid("Tune Rotate PID");
              m_drive.RotatePidTune();
              break;
            case 1:                       // Drive PID
              m_robotLog.WritePid("Tune Drive PID");
              m_drive.DrivePidTune();
              m_drive.RotateInit(0, 0.6, true);
              break;
            case 2:                       // Elevator PID
              m_robotLog.WritePid("Tune Elevator PID");
              break;
            case 3:                       // Shoulder PID
              m_robotLog.WritePid("Tune Shoulder PID");
              break;
            case 4:                       // Wrist PID
              m_robotLog.WritePid("Tune Wrist PID");
              break;
            default:;
        }

        m_tunePID.SetOutputRamp(0.1, 0.05);
        m_tunePID.SetSetpoint(setpoint, 0);
        m_tunePID.SetOutputRange(-m_dashboard.GetDashValue(dvPidMaxOut),m_dashboard.GetDashValue(dvPidMaxOut));

        m_tunePID.SetCoefficient  ('P', m_dashboard.GetDashValue(dvPthreshold),
                                        m_dashboard.GetDashValue(dvPabove),
                                        m_dashboard.GetDashValue(dvPbelow));
                                        
        m_tunePID.SetCoefficient  ('I', m_dashboard.GetDashValue(dvIthreshold),
                                        m_dashboard.GetDashValue(dvIabove),
                                        m_dashboard.GetDashValue(dvIbelow));
                                        
        m_tunePID.SetCoefficient  ('D', m_dashboard.GetDashValue(dvDthreshold),
                                        m_dashboard.GetDashValue(dvDabove),
                                        m_dashboard.GetDashValue(dvDbelow));
        m_tunePID.Reset();
        m_tunePID.SetShowOutput(true);

        sprintf(message, "Setpoint=%f  Maximum Output=%f", setpoint, m_dashboard.GetDashValue(dvPidMaxOut));
        m_robotLog.WritePid(message);

        sprintf(message, "P: Threshold=%f  Above=%f  Below=%f", m_dashboard.GetDashValue(dvPthreshold),
                                                                 m_dashboard.GetDashValue(dvPabove),
                                                                 m_dashboard.GetDashValue(dvPbelow));
        m_robotLog.WritePid(message);

        sprintf(message, "I: Threshold=%f  Above=%f  Below=%f", m_dashboard.GetDashValue(dvIthreshold),
                                                                 m_dashboard.GetDashValue(dvIabove),
                                                                 m_dashboard.GetDashValue(dvIbelow));
        m_robotLog.WritePid(message);

        sprintf(message, "D: Threshold=%f  Above=%f  Below=%f", m_dashboard.GetDashValue(dvDthreshold),
                                                                 m_dashboard.GetDashValue(dvDabove),
                                                                 m_dashboard.GetDashValue(dvDbelow));
        m_robotLog.WritePid(message);
      }

      switch(tunePID) {
        case 0:                                                                 //Rotate PID
          m_drive.ArcadeDrive(0, m_tunePID.Calculate(m_drive.GetHeading()));
          break;
        case 1:                                                                 //Drive PID
          m_drive.ArcadeDrive(m_tunePID.Calculate(m_drive.GetDistance(Drive::ueCurrentEncoder)), m_drive.RotateExec());
          break;
        case 2:                                                                 //Elevator PID
          m_elevator.SetElevatorMotor(m_tunePID.Calculate(m_elevator.GetElevatorPosition()));
          break;
        case 3:                                                                 // Shoulder PID
          m_arm.SetShoulderMotor(m_tunePID.Calculate(m_arm.GetShoulderDegrees()));
          break;
        case 4:                                                                 // Wrist PID
          m_arm.SetWristMotor(m_tunePID.Calculate(m_arm.GetWristDegrees()));
          break;
      }
  } else {
    tunePID = -1;
    m_drive.ArcadeDrive(0,0);
    m_elevator.SetElevatorMotor(0);
    m_arm.SetShoulderMotor(0);
    m_arm.SetWristMotor(0);
  }
}

void Robot::SetDashRobotValues() {
  m_dashboard.SetRobotValue(rvDriveGyro, m_drive.GetHeading());
  m_dashboard.SetRobotValue(rvDriveEncoderL, m_drive.GetDistance(Drive::ueLeftEncoder));
  m_dashboard.SetRobotValue(rvDriveEncoderR, m_drive.GetDistance(Drive::ueRightEncoder));
  m_dashboard.SetRobotValue(rvDriveAmpsLeft1, m_pdp.GetCurrent(pdpDriveLeft1));
  m_dashboard.SetRobotValue(rvDriveAmpsLeft2, m_pdp.GetCurrent(pdpDriveLeft2));
  m_dashboard.SetRobotValue(rvDriveAmpsRight1, m_pdp.GetCurrent(pdpDriveRight1));
  m_dashboard.SetRobotValue(rvDriveAmpsRight2, m_pdp.GetCurrent(pdpDriveRight2));
  m_dashboard.SetRobotValue(rvElevatorPosition, m_elevator.GetElevatorPosition());
  m_dashboard.SetRobotValue(rvElevatorSetpoint, m_elevator.GetElevatorSetpoint());
  m_dashboard.SetRobotValue(rvElevatorAmps, m_pdp.GetCurrent(pdpElevator));
  m_dashboard.SetRobotValue(rvShoulderPosition, m_arm.GetShoulderDegrees());
  m_dashboard.SetRobotValue(rvShoulderSetpoint, m_arm.GetShoulderSetpoint());
  m_dashboard.SetRobotValue(rvShoulderAmps, m_pdp.GetCurrent(pdpShoulder));
  m_dashboard.SetRobotValue(rvWristPosition, m_arm.GetWristDegrees());
  m_dashboard.SetRobotValue(rvWristSetpoint, m_arm.GetWristSetpoint());
  m_dashboard.SetRobotValue(rvWristAmps, m_pdp.GetCurrent(pdpWrist));
  m_dashboard.SetRobotValue(rvVisionStatus, (double)m_vision.GetSearchState());
  m_dashboard.SetRobotValue(rvVisionSelect, (double)m_vision.GetTargetSelect());
  m_dashboard.SetRobotValue(rvVisionAngle, m_vision.GetTargetAngle());
  m_dashboard.SetRobotValue(rvVisionDistance, m_vision.GetTargetDistance());

  m_dashboard.SetRobotStatus(rsFloorFront, m_elevator.FloorDetected(Elevator::fsFront));
  m_dashboard.SetRobotStatus(rsFloorRear, m_elevator.FloorDetected(Elevator::fsRear));
  m_dashboard.SetRobotStatus(rsCargo, m_arm.GetCargoDetected());
}

void Robot::StartDriverCamera() {
  frc::CameraServer* cameraServer = frc::CameraServer::GetInstance();

  m_cameraDriver = cameraServer->StartAutomaticCapture(0);
  m_cameraDriver.SetResolution(320, 240);
  m_cameraDriver.SetFPS(20);
  m_cameraDriver.SetBrightness(30);
  m_cameraDriver.SetExposureAuto();

  m_cameraServer.SetSource(m_cameraDriver);
  m_cameraServer = cameraServer->GetServer();
}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
