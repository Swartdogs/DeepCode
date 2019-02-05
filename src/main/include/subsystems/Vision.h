
#pragma once

#include <frc/WPILib.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

class Vision {
 public:
   typedef enum{ssDone, ssLooking, ssNoTarget, ssTargetFound} SearchState;
   typedef enum{tsBest, tsLeft, tsRight} TargetSelect;

  Vision();
  void          FindTarget(Vision::TargetSelect targetSelect = tsBest);
  bool          GetCameraImage(cv::Mat& image);
  SearchState   GetSearchState();
  double        GetTargetAngle();
  double        GetTargetDistance();
  void          InitVision();
  void          SearchResults(bool targetFound, double targetAngle, double targetDistance);
  static void   TargetSearch(Vision* host, Vision::TargetSelect targetSelect);

private:
  char          m_buffer[100];
  SearchState   m_searchState;
  cs::CvSink    m_sink;
  cs::UsbCamera m_camera;
  cs::VideoSink m_server;
  double        m_targetAngle;
  double        m_targetDistance;
  std::thread*  m_task;

};
