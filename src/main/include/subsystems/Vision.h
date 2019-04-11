
#pragma once

#include <frc/WPILib.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

class Vision {
 public:
   typedef enum{cmTarget, cmDriver} CameraMode;
   typedef enum{ssDone, ssLooking, ssNoImage, ssNoTarget, ssTargetFound} SearchState;
   typedef enum{tsBest, tsLeft, tsRight} TargetSelect;

  Vision();
  void          AddDistanceOffset(double offset);
  void          FindTarget(Vision::TargetSelect targetSelect = tsBest);
  bool          GetCameraImage(cv::Mat& image);
  int           GetContoursFound();
  SearchState   GetSearchState();
  double        GetTargetAngle();
  double        GetTargetDistance();
  TargetSelect  GetTargetSelect();
  double        GetTravelDistance();
  void          InitVision();
  bool          InTargetMode();
  void          NewCameraMode(CameraMode mode);
  void          SearchResults(SearchState state, double targetAngle, double targetDistance, int contoursFound);
  void          SetCameraMode(CameraMode mode);

  // Functions executed in seperate Threads

  static void   ConfigureCamera(Vision* host, cs::UsbCamera* myCamera, Vision::CameraMode mode);
  static void   TargetSearch(Vision* host, Vision::TargetSelect targetSelect);

private:
  char          m_buffer[100];
  cs::UsbCamera m_camera;
  CameraMode    m_cameraMode;
  std::thread*  m_cameraThread;
  int           m_contoursFound;
  SearchState   m_searchState;
  std::thread*  m_searchThread;
  cs::CvSink    m_sink;
  cs::VideoSink m_server;
  double        m_targetAngle;
  double        m_targetDistance;
  TargetSelect  m_targetSelect;
  double        m_travelDistance;
};
