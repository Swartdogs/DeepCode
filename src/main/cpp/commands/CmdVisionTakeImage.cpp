#include "Robot.h"
#include "commands/CmdVisionTakeImage.h"

CmdVisionTakeImage::CmdVisionTakeImage(Vision::TargetSelect targetSelect) {
  m_targetSelect = targetSelect;
}

void CmdVisionTakeImage::Initialize() {
  Robot::m_vision.FindTarget(m_targetSelect);
}

void CmdVisionTakeImage::Execute() {}

bool CmdVisionTakeImage::IsFinished() {
  return (Robot::m_vision.GetSearchState() != Vision::ssLooking); 
  }

void CmdVisionTakeImage::End() {}

void CmdVisionTakeImage::Interrupted() {}
