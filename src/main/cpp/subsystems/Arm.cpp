#include "subsystems/Arm.h"
#include "Robot.h"

Arm::Arm() : Subsystem("Arm") {
  Robot::m_robotLog.Write("Arm:      INIT", false);

  m_shoulderPot.SetAverageBits(2);
  m_shoulderPot.SetOversampleBits(0);
  m_wristPot.SetAverageBits(2);
  m_wristPot.SetOversampleBits(0);

  m_intakeMode        = imOff;
  m_handModeSwitch    = hmNone;
  m_handModeRobot     = hmNone;
  m_hatchState        = hsGrab;
  m_manualDrive       = false;
  m_ignoreCargo       = false;
  m_presetPosition    = apUnknown;
  m_shoulderNext      = -1;
  m_shoulderSetpoint  = GetShoulderDegrees();
  m_usePreset         = false;
  m_wristNext         = -1;
  m_wristSetpoint     = GetWristDegrees();

  m_armPosition       = apUnknown;
  m_shoulderPosition  = apUnknown;
  m_wristPosition     = apUnknown;

  m_shoulderPID.SetCoefficient('P', 0, 0.04, 0);
  m_shoulderPID.SetCoefficient('I', 15, 0, 0.001);
  m_shoulderPID.SetCoefficient('D', 5, 0.1, 0);
  m_shoulderPID.SetInputRange(0, 125);
  m_shoulderPID.SetOutputRange(-0.4, 0.7);
  m_shoulderPID.SetOutputRamp(0.10, 0.05);
  m_shoulderPID.SetSetpointDeadband(1.0); 
  m_shoulderPID.SetSetpoint(m_shoulderSetpoint, m_shoulderSetpoint);

  m_wristPID.SetCoefficient('P', 0, 0.01, 0);
  m_wristPID.SetCoefficient('I', 20, 0, 0.0008);
  m_wristPID.SetCoefficient('D', 5, 0.02, 0);
  m_wristPID.SetInputRange(0, 250);
  m_wristPID.SetOutputRange(-0.6, 0.6);
  m_wristPID.SetOutputRamp(0.20, 0.05);
  m_wristPID.SetSetpointDeadband(1.0); 
  m_wristPID.SetSetpoint(m_wristSetpoint, m_wristSetpoint);

  m_handBottom.SetInverted(true);
  m_handTop.SetInverted(true);
  m_shoulderMotor.SetInverted(true);

  m_solHandHatch.Set(false);
  m_solHandCargo.Set(false);
  m_solHatch.Set(false);
}

void Arm::InitDefaultCommand() {
}

void Arm::Periodic() {
  static bool inManualDrive   = false;
  static int  timer           = 0;

  double      shoulderNow     = GetShoulderDegrees();
  double      shoulderPower   = m_shoulderPID.Calculate(shoulderNow);;
  double      wristNow        = GetWristDegrees();
  double      wristPower      = m_wristPID.Calculate(wristNow);
  double      topPower        = 0;
  double      bottomPower     = 0;

  if (m_wristPID.AtSetpoint()) m_ignoreCargo = false;

  switch(m_intakeMode) {
    case imOff:
      timer = 0;
      break;

    case imIn:
      if(m_ignoreCargo || m_cargoSensor.Get()) {                // Wrist position not at setpoint or No Cargo
        topPower = Robot::m_dashboard.GetDashValue(dvCargoSpeedIn);
        bottomPower = topPower;
      } else {
        SetIntakeMode(imOff);
        Robot::m_dashboard.SetRobotStatus(rsCargoLoaded, true);
      }
      break; 

    case imOut:
      if(timer < Robot::m_dashboard.GetDashValue(dvCargoEjectTime)) {
        topPower = Robot::m_dashboard.GetDashValue(dvCargoSpeedOut);
        bottomPower = topPower;
        
        if (m_cargoSensor.Get()) timer++;
        else timer = 0;

      } else {
        SetIntakeMode(imOff);        
      }
      break;

    case imRotate:
      if(timer < Robot::m_dashboard.GetDashValue(dvCargoRotateTime)) {
        timer++;

        topPower = Robot::m_dashboard.GetDashValue(dvCargoSpeedRotate);
        bottomPower = -PowerLimit(topPower * Robot::m_dashboard.GetDashValue(dvCargoRotateRatio));
      } else {
        SetIntakeMode(imOff);
      }
      break;

    default:;
  }

  if (m_manualDrive) {
    if (!inManualDrive) {
      inManualDrive = true;
      m_armPosition = apUnknown;
      m_shoulderNext = -1;
      m_wristNext = -1;
      SetShoulderPosition(shoulderNow);
      SetWristPosition(wristNow);

      sprintf(Robot::message,"Arm:      Manual START  Shoulder=%4.1f  Wrist=%4.1f", shoulderNow, wristNow);
      Robot::m_robotLog.Write(Robot::message);
    }

    double joyShoulder  = Robot::m_oi.GetArmJoystickY() * 0.5;
    double joyWrist     = Robot::m_oi.GetArmJoystickX() * 0.5;

//    printf("Shoulder=%f  Wrist=%f\n", joyShoulder, joyWrist);

    if (joyShoulder < 0) {
      if (shoulderNow < Robot::m_dashboard.GetDashValue(dvShoulderMin) + 4) {
        shoulderPower = 0;
      } else {
        shoulderPower = joyShoulder;
      } 

      SetShoulderPosition(shoulderNow);
      
    } else if (joyShoulder > 0) {
      if ((shoulderNow < Robot::m_dashboard.GetDashValue(dvShoulderMax) - 4)
           && shoulderPower < joyShoulder) {
        shoulderPower = joyShoulder;
        SetShoulderPosition(shoulderNow);
      }
    } 

    if (joyWrist < 0) {
      if (wristNow < Robot::m_dashboard.GetDashValue(dvWristMin) + 4) {
        wristPower = 0;
      } else {
        wristPower = joyWrist;
      } 

      SetWristPosition(wristNow);
      
    } else if (joyWrist > 0) {
      if ((wristNow < Robot::m_dashboard.GetDashValue(dvWristMax) - 4)
           && wristPower < joyWrist) {
        wristPower = joyWrist;
        SetWristPosition(wristNow);
      }
    } 
    
  } else {
    if (inManualDrive) {
      inManualDrive = false;
      sprintf(Robot::message,"Arm:      Manual END    Shoulder=%4.1f  Wrist=%4.1f", shoulderNow, wristNow);
      Robot::m_robotLog.Write(Robot::message);
    }

    if(m_shoulderSetpoint < 3 && GetShoulderDegrees() < 3) shoulderPower = 0;
    if (m_wristSetpoint < 3 && GetWristDegrees() < 3) wristPower = 0;

    if (m_shoulderNext >= 0) {                                                  // Shoulder waiting for Wrist
      if (wristNow < Robot::m_dashboard.GetDashValue(dvWristClear) || WristAtSetpoint()) {    // Wrist Clear or Stopped
        SetShoulderPosition(m_shoulderNext, m_armPosition);
        m_shoulderNext = -1;
      }
    } else if (m_wristNext >= 0) {                                              // Wrist waiting for Shoulder
      if (shoulderNow > Robot::m_dashboard.GetDashValue(dvShoulderClear) || ShoulderAtSetpoint()) {   // Shoulder Clear or Stopped
        SetWristPosition(m_wristNext, m_armPosition);
        m_wristNext = -1;
      } else {
        double newSetpoint = m_wristSetpoint + 5.0;
        
        if (newSetpoint >= m_wristNext) {
          SetWristPosition(m_wristNext, m_armPosition);
          m_wristNext = -1;
        } else {
          m_wristSetpoint = newSetpoint;
          m_wristPID.SetSetpoint(m_wristSetpoint, GetWristPosition(), false);
        }
      }
    }

    if (m_armPosition == apPickup && m_handModeRobot == hmCargo) {
      if (m_wristSetpoint == Robot::m_dashboard.GetDashValue(dvWCPickup) && WristAtSetpoint()) {
        if (m_shoulderSetpoint != Robot::m_dashboard.GetDashValue(dvSCPickup)) {
          SetShoulderPosition(Robot::m_dashboard.GetDashValue(dvSCPickup), m_armPosition);
        }
      }
    }
  }

  m_shoulderMotor.Set(shoulderPower);
  m_wristMotor.Set(wristPower);
  m_handTop.Set(topPower);
  m_handBottom.Set(bottomPower);
}

std::string Arm::GetArmPositionName(ArmPosition position) {
  std::string name = "";

  switch (position) {
      case apUnknown:     name = "Unknown";         break;
      case apTravel:      name = "Travel";          break;
      case apPickup:      name = "Pickup";          break;
      case apLoad:        name = "Load";            break;
      case apLow:         name = "Low";             break;
      case apMid:         name = "Mid";             break;
      case apHigh:        name = "High";            break;
      case apCargoShip:   name = "Cargo Ship";      break;
      case apCargoCatch:  name = "Cargo Catch";     break;
      case apWait:        name = "Wait for Clear";  break;
      case apPreset:      name = "Preset";          break;
  }

  return name;
}

bool Arm::GetCargoDetected() {
  return !m_cargoSensor.Get();
}

Arm::HandMode Arm::GetHandModeSwitch() {
  return m_handModeSwitch;
}

std::string Arm::GetHandModeName(HandMode mode) {
 std::string name = "";

  switch (mode) {
      case hmCargo:    name = "Cargo";    break;
      case hmHatch:    name = "Hatch";    break;
  }

  return name; 
}

Arm::HatchState Arm::GetHatchState () {
  return m_hatchState;
}

std::string Arm::GetHatchStateName(HatchState state) {
   std::string name = "";

  switch (state) {
    case hsGrab:       name = "Grab";       break;
    case hsRelease:    name = "Release";    break;
  }

  return name; 
}

Arm::IntakeMode Arm::GetIntakeMode(){
  return m_intakeMode;
}

std::string Arm::GetIntakeModeName(IntakeMode mode) {
  std::string name = "";

  switch (mode) {
    case imOff:     name = "Off";     break;
    case imIn:      name = "In";      break;
    case imOut:     name = "Out";     break;
    case imRotate:  name = "Rotate";  break;
  }

  return name;
}

Arm::ArmPosition Arm::GetPresetPosition() {
  return m_presetPosition;
}

double Arm::GetShoulderDegrees() {
  return (m_shoulderPot.GetAverageValue() / SHOULDER_COUNTS_PER_DEGREE) - Robot::m_dashboard.GetDashValue(dvShoulderOffset);
}

Arm::ArmPosition Arm::GetShoulderPosition(){
  return m_shoulderPosition;
}

double Arm::GetShoulderSetpoint() {
  return m_shoulderSetpoint;
}

double Arm::GetWristDegrees() {
  return Robot::m_dashboard.GetDashValue(dvWristOffset) - (m_wristPot.GetAverageValue() / WRIST_COUNTS_PER_DEGREE);
}

Arm::ArmPosition Arm::GetWristPosition(){
  return m_wristPosition;
}

double Arm::GetWristSetpoint() {
  return m_wristSetpoint;
}

bool Arm::InPresetMode() {
  return m_usePreset;
}

bool Arm::IsDrivenManually() {
  return m_manualDrive;
}

double Arm::PowerLimit(double value) {
  if (value > 1.0) return 1.0;
  else if (value < -1.0) return -1.0;
  return value;
}

void Arm::SetArmPosition(ArmPosition position) {
  if (m_usePreset) {
    m_presetPosition = apUnknown;

    if (m_handModeSwitch == hmHatch) {
      switch (position) {
        case apLow: 
        case apMid:
        case apHigh:
          m_presetPosition = position;
          break;
        default:;
      }
    }
    return;
  }

  m_presetPosition    = apUnknown;
  double shoulderNew  = 0;
  double shoulderNow  = GetShoulderDegrees();
  double wristNew     = 0;
  double wristNow     = GetWristDegrees();

  switch (position) {
    case apTravel:    
      
      shoulderNew   = Robot::m_dashboard.GetDashValue(dvShoulderTravel);
      wristNew      = Robot::m_dashboard.GetDashValue(dvWristTravel);
      SetHandModeRobot(hmCargo);
      SetIntakeMode(imOff);

      break;

    case apPickup:  

      if (m_handModeSwitch == hmCargo) {
        shoulderNew = Robot::m_dashboard.GetDashValue(dvShoulderClear);
        wristNew    = Robot::m_dashboard.GetDashValue(dvWCPickup);
        SetHandModeRobot(hmCargo);
        SetIntakeMode(imIn);
      }
      
      break;

    case apLoad:

      if (m_handModeSwitch == hmCargo) {
        shoulderNew   = Robot::m_dashboard.GetDashValue(dvSCLoad);
        wristNew      = Robot::m_dashboard.GetDashValue(dvWCLoad);
        SetHandModeRobot(hmCargo);
        SetIntakeMode(imIn);
      }

      break;

    case apLow:    

      SetHandModeRobot(m_handModeSwitch);
    
      if (m_handModeSwitch == hmCargo) {
        shoulderNew = Robot::m_dashboard.GetDashValue(dvSCRocketLow);
        wristNew    = Robot::m_dashboard.GetDashValue(dvWCRocketLow);
      } else {
        shoulderNew = Robot::m_dashboard.GetDashValue(dvSHRocketLow);
        wristNew    = Robot::m_dashboard.GetDashValue(dvWHRocketLow);
      }

      break;

    case apMid:   

      SetHandModeRobot(m_handModeSwitch);
    
      if (m_handModeSwitch == hmCargo) {
        shoulderNew = Robot::m_dashboard.GetDashValue(dvSCRocketMid);
        wristNew    = Robot::m_dashboard.GetDashValue(dvWCRocketMid);
      } else {
        shoulderNew = Robot::m_dashboard.GetDashValue(dvSHRocketMid);
        wristNew    = Robot::m_dashboard.GetDashValue(dvWHRocketMid);
      }

      break;

    case apHigh:    

      SetHandModeRobot(m_handModeSwitch);
    
      if (m_handModeSwitch == hmCargo) {
        shoulderNew = Robot::m_dashboard.GetDashValue(dvSCRocketHigh);
        wristNew    = Robot::m_dashboard.GetDashValue(dvWCRocketHigh);
      } else {
        shoulderNew = Robot::m_dashboard.GetDashValue(dvSHRocketHigh);
        wristNew    = Robot::m_dashboard.GetDashValue(dvWHRocketHigh);
      }

      break;

    case apCargoShip: 

      if (m_handModeSwitch == hmCargo) {
        shoulderNew = Robot::m_dashboard.GetDashValue(dvSCCargoShip);
        wristNew = Robot::m_dashboard.GetDashValue(dvWCCargoShip);
        SetHandModeRobot(hmCargo);
      }
      
      break;

    case apCargoCatch:

      if (m_handModeSwitch == hmCargo) {
        shoulderNew = Robot::m_dashboard.GetDashValue(dvSCCatch);
        wristNew = Robot::m_dashboard.GetDashValue(dvWCCatch);
        SetHandModeRobot(hmCargo);
        SetIntakeMode(imIn);
      }
      
      break;

    default:
      return; 
  }

  m_ignoreCargo = true;

  if (shoulderNew == 0 || wristNew == 0) return;                                // No New setpoints

  double shoulderClear = Robot::m_dashboard.GetDashValue(dvShoulderClear);
  double wristClear    = Robot::m_dashboard.GetDashValue(dvWristClear);

  m_armPosition   = position;
  m_shoulderNext  = -1;
  m_wristNext     = -1;

  if (m_shoulderSetpoint != shoulderNew) {                                      // New Shoulder setpoint
    if (shoulderNew > shoulderNow) {                                            // SHOULDER MOVING UP
      SetShoulderPosition(shoulderNew, position);                               // Move Shoulder to New

      if (wristNew < wristClear) {                                              // New Wrist below Clear
        SetWristPosition(wristNew, position);                                   // Move Wrist to New
      } else if (shoulderNew < shoulderClear) {                                 // New Shoulder below Clear
        m_wristNext = wristNew;                                                 // Move Wrist to Clear and wait for Shoulder
        SetWristPosition(wristClear, apWait);
      } else if (shoulderNow > shoulderClear) {                                 // Shoulder already above Clear
        SetWristPosition(wristNew, position);                                   // Move Wrist to New
      } else if (wristNew < 120) {                                              // Wrist will clear due to Shoulder movement
        SetWristPosition(wristNew, position);                                   // Move Wrist to New
      } else {
        m_wristNext = wristNew;                                                 // Move Wrist to Clear and wait for Shoulder
        SetWristPosition(wristClear, apWait);
      }

    } else {                                                                    // SHOULDER MOVING DOWN
      if (shoulderNew > 20) {                                                   // Shoulder New above 20
        SetShoulderPosition(shoulderNew, position);                             // Move Shoulder to New
      } else if (wristNow < wristClear && wristNew < wristClear) {              // Wrist Now and New below Clear
        SetShoulderPosition(shoulderNew, position);                             // Move Shoulder to New
      } else {                                                                  
        m_shoulderNext = shoulderNew;                                           // Move Shoulder to Clear and wait for Wrist
        if (wristNew > wristNow) {                                              // Wrist New above current
          SetShoulderPosition(shoulderClear, apWait);
        } else {                                                                // Wait for Wrist
          SetShoulderPosition(GetShoulderDegrees(), apWait);
        }
      }  

      if (wristNew < wristClear) {                                              // Wrist New below Clear
        SetWristPosition(wristNew, position);                                   // Move Wrist to New
      } else if (shoulderNew >= shoulderClear) {                                // Shoulder New above Clear
        SetWristPosition(wristNew, position);                                   // Move Wrist to New
      } else if (shoulderNow > 20 && shoulderNew > 20) {                        // Shoulder Now above Clear and new > 20
        SetWristPosition(wristNew, position);                                   // Move Wrist to New
      } else {                                                                  // Wrist New and (Shoulder Now or New) below Clear
        m_wristNext = wristNew;                                                 // Move Wrist to Clear and wait for Shoulder
        SetWristPosition(wristClear, apWait);       
      }
    }

  } else if (m_wristSetpoint != wristNew) {                                     // New Wrist Only
    SetWristPosition(wristNew, position);
  }
}

void Arm::SetArmSpeed(bool slowSpeed) {
  if (slowSpeed) {
    m_shoulderPID.SetOutputRange(-0.2, 0.5);
    m_wristPID.SetOutputRange(-0.4, 0.4);
  } else {
    m_shoulderPID.SetOutputRange(-0.3, 0.7);
    m_wristPID.SetOutputRange(-0.6, 0.6);
  }
}

void Arm::SetDrivenManually(bool isManual) {
  m_manualDrive = isManual;
}

void Arm::SetHandModeSwitch(HandMode mode) {
  m_handModeSwitch = mode;
  if (m_armPosition != apTravel) SetHandModeRobot(mode);
}

void Arm::SetHandModeRobot(HandMode mode) {
  if(mode != m_handModeRobot) {
    m_handModeRobot = mode;

    if (m_handModeRobot == hmHatch) {
      m_solHandHatch.Set(true);
      m_solHandCargo.Set(false);
    } else {
      m_solHandHatch.Set(false);
      m_solHandCargo.Set(true);
    }

    Robot::m_dashboard.SetRobotStatus(rsHatchMode, mode == hmHatch);

    sprintf(Robot::message, "Arm:      Hand Mode=%s", GetHandModeName(m_handModeRobot).c_str());
    Robot::m_robotLog.Write(Robot::message);
  }
}

void Arm::SetHatchState(HatchState state) {
  if (state != m_hatchState) {
    m_solHatch.Set(state == hsRelease);
    m_hatchState = state;
    Robot::m_dashboard.SetRobotStatus(rsHatchGrab, state == hsRelease);

    sprintf(Robot::message, "Arm:      Hatch State=%s", GetHatchStateName(m_hatchState).c_str());
    Robot::m_robotLog.Write(Robot::message);
  }
}

void Arm::SetIntakeMode(IntakeMode mode) {
  if (mode != m_intakeMode) {
    m_intakeMode = mode;
    if (m_intakeMode == imIn) Robot::m_dashboard.SetRobotStatus(rsCargoLoaded, false);
    sprintf(Robot::message, "Arm:      Intake Mode=%s", GetIntakeModeName(m_intakeMode).c_str());
    Robot::m_robotLog.Write(Robot::message);
  }
}

void Arm::SetPresetMode(bool usePreset) {
  m_usePreset = usePreset;
}

void Arm::SetShoulderMotor(double speed) {
  m_shoulderMotor.Set(speed);
}

void Arm::SetShoulderPosition(double degrees, ArmPosition position, bool resetPID) {
  if (degrees < Robot::m_dashboard.GetDashValue(dvShoulderTravel)) {
    degrees = Robot::m_dashboard.GetDashValue(dvShoulderTravel);
  } else if (degrees > Robot::m_dashboard.GetDashValue(dvShoulderMax)) {
    degrees = Robot::m_dashboard.GetDashValue(dvShoulderMax);
  }

  m_shoulderSetpoint = degrees;
  m_shoulderPosition = position;

  m_shoulderPID.SetSetpoint(degrees, GetShoulderPosition(), resetPID);
 
  if(position != apUnknown){
    sprintf(Robot::message,"Arm:      Set Shoulder Position=%s (%5.1f)", 
            GetArmPositionName(position).c_str(), degrees);
    Robot::m_robotLog.Write(Robot::message);
  }
}

void Arm::SetWristMotor(double speed) {
  m_wristMotor.Set(speed);
}

void Arm::SetWristPosition(double degrees, ArmPosition position) {
  m_wristSetpoint = degrees;
  m_wristPosition = position;
  
  m_wristPID.SetSetpoint(degrees, GetWristPosition());

  if(position != apUnknown){
    sprintf(Robot::message,"Arm:      Set Wrist Position=%s (%5.1f)", 
            GetArmPositionName(position).c_str(), degrees);
    Robot::m_robotLog.Write(Robot::message);
  }
}

bool Arm::ShoulderAtSetpoint() {
  return m_shoulderPID.AtSetpoint();
}

bool Arm::WristAtSetpoint() {
  return m_wristPID.AtSetpoint();
}