#include "OI.h"
#include <frc/WPILib.h>

#include "Robot.h"
#include "subsystems/Elevator.h"
#include "subsystems/Drive.h"
#include "subsystems/Vision.h"

#include "commands/CmdArmIncrementShoulder.h"
#include "commands/CmdArmSetArmPosition.h"
#include "commands/CmdArmSetHatchState.h"
#include "commands/CmdArmSetSpeed.h"
#include "commands/CmdArmSetHandMode.h"
#include "commands/CmdArmSetIntakeMode.h"
#include "commands/CmdArmSetManual.h"
#include "commands/CmdArmSetPresetMode.h"
#include "commands/CmdCancelClimb.h"
#include "commands/CmdDriveSetGear.h"
#include "commands/CmdDriveSetUseGyro.h"
#include "commands/CmdSandStormAuto.h"
#include "commands/CmdVisionFindTarget.h"
#include "commands/CmdVisionToggleMode.h"
#include "commands/GrpClimb.h"
#include "commands/GrpEndHatchRight.h"
#include "commands/GrpEndHatchLeft.h"
#include "commands/GrpSideHatchLeft1.h"
#include "commands/GrpGoToTarget.h"
#include "commands/GrpSideHatchRight1.h"
#include "commands/GrpRocketLeftL1.h"
#include "commands/GrpRocketLeftL2.h"

OI::OI() {
  Robot::m_robotLog.Write("OI:       INIT", false);

  m_buttonDrive1.WhenPressed        (new GrpGoToTarget());
  m_buttonDrive2.WhenPressed        (new CmdDriveSetGear(Drive::spLow));
  m_buttonDrive2.WhenReleased       (new CmdDriveSetGear(Drive::spHigh));
  m_buttonDrive6.WhenPressed        (new CmdVisionFindTarget(Vision::tsBest));
//  m_buttonDrive7.WhenPressed        (new CmdVisionToggleMode());
  m_buttonDrive7.WhenPressed        (new CmdDriveSetUseGyro(true));
  m_buttonDrive7.WhenReleased       (new CmdDriveSetUseGyro(false));
  m_buttonDrive10.WhenPressed       (new CmdCancelClimb());

  m_buttonArm1.WhenPressed          (new CmdArmSetManual(true));
  m_buttonArm1.WhenReleased         (new CmdArmSetManual(false));
  m_buttonArm2.WhenPressed          (new CmdArmSetArmPosition(Arm::apCargoCatch));
  m_buttonArm8.WhenPressed          (new CmdArmSetArmPosition(Arm::apDefense));
  m_buttonArm10.WhenPressed         (new CmdArmIncrementShoulder(-1.0));
  m_buttonArm11.WhenPressed         (new CmdArmIncrementShoulder(1.0));

  m_buttonBox1.WhenPressed          (new CmdArmSetArmPosition(Arm::apHigh));
  m_buttonBox5.WhenPressed          (new CmdArmSetArmPosition(Arm::apMid));
  m_buttonBox9.WhenPressed          (new CmdArmSetArmPosition(Arm::apLow));

  m_buttonBox2.WhenPressed          (new CmdArmSetArmPosition(Arm::apCargoShip));
  m_buttonBox6.WhenPressed          (new CmdArmSetArmPosition(Arm::apPickup));
  m_buttonBox10.WhenPressed         (new CmdArmSetArmPosition(Arm::apTravel));

  m_buttonBox3.WhenPressed          (new CmdArmSetHatchState(Arm::hsGrab));
  m_buttonBox7.WhenPressed          (new CmdArmSetHatchState(Arm::hsRelease));
  m_buttonBox11.WhenPressed         (new CmdArmSetPresetMode(true));
  m_buttonBox11.WhenReleased        (new CmdArmSetPresetMode(false));

  m_buttonBox4.WhenPressed          (new CmdArmSetIntakeMode(Arm::imOut));
  m_buttonBox8.WhenPressed          (new CmdArmSetSpeed(true));
  m_buttonBox8.WhenReleased         (new CmdArmSetSpeed(false));
  m_buttonBox12.WhenPressed         (new CmdArmSetIntakeMode(Arm::imIn));

  m_internalAuto.WhenPressed        (new CmdSandStormAuto());
  m_internalHandMode.WhenPressed    (new CmdArmSetHandMode(Arm::hmHatch));
  m_internalHandMode.WhenReleased   (new CmdArmSetHandMode(Arm::hmCargo));
  m_internalLevel2.WhenPressed      (new GrpClimb(Elevator::epLevel2));
  m_internalLevel3.WhenPressed      (new GrpClimb(Elevator::epLevel3));

  m_autoGroup = nullptr;
}

double OI::ApplyDeadband(double joystickValue, double deadband) {     // Return Value with Deadband applied
  deadband = fabs(deadband);

  if (joystickValue > deadband) {
    return ((joystickValue - deadband) / (1 - deadband));
  } else if (joystickValue < -deadband) {
    return ((joystickValue + deadband) / (1 - deadband));
  }
  
  return 0;
}

bool OI::DriverCancel() {                                             // Used to Cancel Command Group based on Driver input 
  return (fabs(m_driveJoystick.GetY()) > 0.3 || fabs(m_driveJoystick.GetX()) > 0.3);
}

double OI::GetArmJoystickX() {                                        // Return Arm X with Deadband applied
  return ApplyDeadband(m_armJoystick.GetX(), 0.15);
}

double OI::GetArmJoystickY() {                                        // Return Arm Y with Deadbanc applied
  return ApplyDeadband(m_armJoystick.GetY(), 0.15);
}

double OI::GetDriveJoystickX() {                                      // Return Drive X with Deadband applied
  return ApplyDeadband(m_driveJoystick.GetX(), 0.05);
}

double OI::GetDriveJoystickY() {                                      // Return Drive Y with Deadband applied
  return -ApplyDeadband(m_driveJoystick.GetY(), 0.05);
}

bool OI::InHatchMode() {                                              // Return TRUE if Hand Switch in Hatch position
  return (m_buttonBox.GetX() < -0.5);
}

void OI::Periodic() {                                                 // Determine state of Internal Buttons (Called every 20ms)
  frc::DriverStation* ds;

  m_internalAuto.SetPressed     (ds->IsAutonomous() && m_buttonDrive8.Get());
  m_internalHandMode.SetPressed (m_buttonBox.GetX() < -0.5);
  m_internalLevel2.SetPressed   (ds->IsOperatorControl() && m_buttonDrive8.Get() && m_buttonArm7.Get());
  m_internalLevel3.SetPressed   (ds->IsOperatorControl() && m_buttonDrive8.Get() && m_buttonArm6.Get());
}

void OI::SandStormAutoCancel() {                                      // Cancel the Auto Command Group
  if (m_autoGroup != nullptr) m_autoGroup->Cancel();
}

void OI::SandStormAutoInit() {                                        // Start an Auto Command Group
  std::string name = "";

  switch ((int)Robot::m_dashboard.GetDashValue(dvAutoHatchPlace)) {
    case 1:   m_autoGroup = new GrpEndHatchLeft();
              name = "Cargo Ship: End Hatch Left";
              break; 
    case 2:   m_autoGroup = new GrpEndHatchRight();
              name = "Cargo Ship: End Hatch Right";
              break;
    case 3:   m_autoGroup = new GrpSideHatchLeft1(48, 0.5);
              name = "Cargo Ship: Side Hatch Left 1 (L1)";
              break;
    case 4:   m_autoGroup = new GrpSideHatchLeft1(92, 0.25);
              name = "Cargo Ship: Side Hatch Left 1 (L2)";
              break;
    case 5:   m_autoGroup = new GrpSideHatchRight1(48, 0.5);
              name = "Cargo Ship: Side Hatch Right 1 (L1)";
              break;
    case 6:   m_autoGroup = new GrpSideHatchRight1(92, 0.25);
              name = "Cargo Ship: Side Hatch Right 1 (L2)";
              break;
    case 7:   m_autoGroup = new GrpRocketLeftL1();
              name = "Rocket: Left (L1)";
              break;
    case 8:   m_autoGroup = new GrpRocketLeftL2();
              name = "Rocket: Left (L2)";
              break;
    case 9:   //m_autoGroup = new GrpRocketLeft();
              name = "Rocket: Right (L1)";
              break;
    case 10:  //m_autoGroup = new GrpRocketLeft();
              name = "Rocket: Right (L2)";
              break;
    default:  m_autoGroup = nullptr;
  }

  if (m_autoGroup != nullptr) {
    Robot::m_drive.ResetGyro();

    sprintf(Robot::message, "SS Auto:  INIT  %s", name.c_str());
    Robot::m_robotLog.Write(Robot::message);

    m_autoGroup->Start();
  }
}

bool OI::SandStormAutoRunning() {                                     // Return TRUE if an Auto Command Group is running
  if (m_autoGroup == nullptr) {
    return false;
  } else {
    return m_autoGroup->IsRunning();
  }
}
