
#include "subsystems/Vision.h"
#include "Robot.h"
Vision::Vision() {
    m_task = nullptr;
    m_searchState = ssDone;
    m_targetAngle = 0;
    m_targetDistance = 0;
}

void Vision::FindTarget() {

    m_targetAngle = 0;
    m_targetDistance = 0;
    m_searchState = ssLooking;

    m_task = new std::thread(TargetSearch, this);
    m_task->detach();
}

bool Vision::GetCameraImage(cv::Mat& image) {
    if(m_sink.GrabFrame(image) == 0) return false;
    return true;
}

Vision::SearchState Vision::GetSearchState(){
    return m_searchState;
}

double Vision::GetTargetDistance(){
    return m_targetDistance;
}

double Vision::GetTargetAngle(){
    return m_targetAngle;
}

void Vision::InitVision(){
    frc::CameraServer* cameraServer = frc::CameraServer::GetInstance();
    m_camera = cameraServer->StartAutomaticCapture("aimCamera", 0);
    m_camera.SetResolution(320, 240);
    m_camera.SetFPS(20);
    m_camera.SetBrightness(20);
    m_camera.SetExposureManual(10);

    m_sink = cameraServer->GetVideo("aimCamera");

    
    m_server.SetSource(m_camera);
    m_server = cameraServer->GetServer();
}

void Vision::SearchResults(bool targetFound, double targetAngle, double targetDistance) {
    if(targetFound) {
        m_searchState = ssTargetFound;
        m_targetAngle = targetAngle;
        m_targetDistance = targetDistance;

        sprintf(m_log, "Vision: Target Angle=%5.1f  Distance=%5.0f", m_targetAngle, m_targetDistance);
    } else {
        m_searchState = ssNoTarget;
    }

}

 void Vision::TargetSearch(Vision* host){
     struct visionTape {
         float  angle;
         int    x;
         int    height;
         int    width;
     };
     struct visionTarget {
         int    area;
         double angle;
         double distance;
     };

     cv::Mat imageA;

    if(host->GetCameraImage(imageA)){
        cv::Mat imageB;
        cv::cvtColor(imageA, imageB, cv::COLOR_BGR2HSV);

        cv::inRange(imageB, cv::Scalar(0, 100, 130), cv::Scalar(180, 255, 255), imageA);

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        contours.clear(); 

        cv::findContours(imageA, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

        imageA.deallocate();
        imageB.deallocate();

        printf("Contours Found=%i\n", contours.size());
    
        std::vector<visionTape> tapes;
        tapes.clear();

        for (std::vector<cv::Point> contour: contours){
            cv::RotatedRect rotatedRect = cv::minAreaRect(contour);
            float rectAngle = rotatedRect.angle;
            if(rectAngle < -45) rectAngle +=90;

            if (fabs(rectAngle) > 5 && fabs(rectAngle) < 25) {
                cv::Rect rect = cv::boundingRect(contour);
                
                if(rect.width >= 10 && rect.height >=20){
                    visionTape tape;
                    tape.angle  = rectAngle;
                    tape.x = rect.x;
                    tape.height = rect.height;
                    tape.width = rect.width;

                    int i = 0;
                    for(i = 0; i < tapes.size(); i++){
                        if(tapes[i].x > tape.x) break;
                    }

                    std::vector<visionTape>::iterator it;
                    it = tapes.begin() + i;
                    tapes.insert(it, tape);
                }
            }
        }

        std::vector<visionTarget> targets;
        targets.clear();
        int leftTape = -2;

        for (int i = 0; i < tapes.size(); i++){
            printf("Tape %i: Angle %f rectX=%i rectHeight=%i rectWidth= %i\n", i, tapes[i].angle,
                     tapes[i].x, tapes[i].height, tapes[i].width);
            
            if (tapes[i].angle > 0){
                leftTape = i;
            } else if (leftTape == i - 1){
                printf("Target Found: Tapes %i and %i\n", i - 1, i);

                int     targetHeight = std::max(tapes[i-1].height, tapes[i].height);
                int     targetWidth = tapes[i].x - tapes[i - 1].x + tapes[i].width;
                double  targetCenter = (((double)targetWidth / 2) + (double)tapes[i - 1].x - 160) / 160;

            }
        }
    } else{
        imageA.deallocate();
        host->SearchResults(false, 0, 0);
    }
 }