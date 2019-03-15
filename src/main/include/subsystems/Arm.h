#pragma once
#include <frc/WPILib.h>
#include <frc/commands/Subsystem.h>
#include "RobotMap.h"
#include "PIDControl.h"

class Arm : public frc::Subsystem {
  public:
    typedef enum {
      apUnknown,
      apTravel,
      apPickup,
      apLoad,
      apLow,
      apMid,
      apHigh,
      apCargoShip,
      apWait,
      apByHand
    } ArmPosition;

    typedef enum {
      hmCargo,
      hmHatch
    } HandMode;

    typedef enum {
      hsRelease,
      hsGrab
    } HatchState;

    typedef enum{
      imOff,
      imIn,
      imOut,
      imRotate
    } IntakeMode;

    Arm();
    void InitDefaultCommand() override;    
    void Periodic() override;
    
    std::string   GetArmPositionName(ArmPosition position);
    bool          GetCargoDetected(); 
    HandMode      GetHandMode();
    std::string   GetHandModeName(HandMode mode);
    HatchState    GetHatchState();
    std::string   GetHatchStateName(HatchState state);
    IntakeMode    GetIntakeMode();
    std::string   GetIntakeModeName(IntakeMode mode);
    double        GetShoulderDegrees();
    ArmPosition   GetShoulderPosition();
    double        GetShoulderSetpoint();
    double        GetWristDegrees();
    ArmPosition   GetWristPosition();
    double        GetWristSetpoint();
    bool          IsDrivenManually();
    void          SetArmPosition(ArmPosition position);
    void          SetArmSpeed(bool slowSpeed);
    void          SetDrivenManually(bool isManual);
    void          SetHandMode(HandMode mode, bool fromSwitch = false);
    void          SetHatchState(HatchState state);
    void          SetIntakeMode(IntakeMode mode);
    void          SetShoulderMotor(double speed);
    void          SetShoulderPosition(double degrees, ArmPosition position = apUnknown, bool resetPID = true);
    void          SetWristMotor(double speed);
    void          SetWristPosition(double degrees, ArmPosition position = apUnknown);
    bool          ShoulderAtSetpoint();
    bool          WristAtSetpoint();

  private:
    const double      SHOULDER_COUNTS_PER_DEGREE = 12;
    const double      WRIST_COUNTS_PER_DEGREE = 12;

    double            PowerLimit(double value);

    bool              m_manualDrive;
    bool              m_ignoreCargo;
    double            m_shoulderNext;
    double            m_shoulderSetpoint;
    double            m_wristNext;
    double            m_wristSetpoint;

    ArmPosition       m_armPosition;
    ArmPosition       m_shoulderPosition;
    ArmPosition       m_wristPosition;
  
    HandMode          m_handMode;

    HatchState        m_hatchState;

    IntakeMode        m_intakeMode;

    frc::VictorSP     m_shoulderMotor{pwmShoulder};
    frc::VictorSP     m_wristMotor{pwmWrist};
    frc::VictorSP     m_handTop{pwmHandTop};
    frc::VictorSP     m_handBottom{pwmHandBottom};

    frc::DigitalInput m_cargoSensor{dioCargoSensor};

    frc::AnalogInput  m_shoulderPot{aioShoulderPot};
    frc::AnalogInput  m_wristPot{aioWristPot};

    PIDControl        m_shoulderPID{"Shoulder"};
    PIDControl        m_wristPID{"Wrist"};

    frc::Solenoid     m_solHandHatch{solHandHatch};
    frc::Solenoid     m_solHandCargo{solHandCargo};
    frc::Solenoid     m_solHatch{solHatch};
};
