#pragma once

#include "RobotMap.h"
#include "frc/WPILib.h"
#include <frc/commands/Subsystem.h>
#include "PIDControl.h"
#include "ctre/phoenix.h"

class Drive : public frc::Subsystem {
 public:
  typedef enum {
    spLow,
    spHigh
  }ShifterPosition;

  typedef enum {
    ueLeftEncoder,
    ueRightEncoder,
    ueBothEncoders,
    ueCurrentEncoder
  }UseEncoder;
  
  Drive();
  ~Drive();

  void              InitDefaultCommand() override;
  void              Periodic() override;

  void              ArcadeDrive(double drive, double rotate);
  double            DriveExec();
  void              DriveInit(double distance, double heading, double maxSpeed, double minSpeed,
                              bool resetEncoder, bool resetGyro, bool driveArc = false);
  bool              DriveIsFinished();
  void              DrivePidTune();
  double            GetDistance (UseEncoder encoder);
  double            GetHeading(); 
  ShifterPosition   GetShifterPosition(); 
  std::string       GetShifterPositionName(ShifterPosition position);
  void              ResetGyro();
  double            RotateExec();
  void              RotateInit(double heading, double maxSpeed, bool resetGyro);
  bool              RotateIsFinished();
  void              RotatePidTune();
  void              SetBrakeMode(bool brakesOn);
  void              SetDriveEnable(bool enable); 
  void              SetDriveInUse(bool inUse);
  void              SetShifter(ShifterPosition position);

  // Function executed in seperate Thread

  static void       ConfigureBrakeMode(Drive* host, bool brakesOn);

  WPI_VictorSPX       m_driveLeft1{canDriveLeft1};
  WPI_VictorSPX       m_driveLeft2{canDriveLeft2};
  WPI_VictorSPX       m_driveRight1{canDriveRight1};
  WPI_VictorSPX       m_driveRight2{canDriveRight2};

private:
  std::thread*        m_brakeThread;
  bool                m_driveEnable;
  bool                m_driveInUse; 

  frc::ADXRS450_Gyro  m_gyro{};

  frc::Encoder        m_encoderLeft{dioEncoderLeftChannelA, dioEncoderLeftChannelB, false};
  frc::Encoder        m_encoderRight{dioEncoderRightChannelA, dioEncoderRightChannelB, true};

  PIDControl          m_drivePID{"Drive"};
  PIDControl          m_rotatePID{"Rotate"};

  ShifterPosition     m_shifterPosition;

  frc::Solenoid       m_solShifter{solShifter};

  UseEncoder          m_useEncoder;
};