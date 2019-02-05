
#include "subsystems/Vision.h"
#include "Robot.h"

// Target Angle:	Tan = x / y where x is the normalized location of the target center (-1 to 1)
//					When:  x = 1 when the Target Angle is 1/2 of camera view angle  	(Axis=48.2/2; LifeCam=50.7/2)
//						   y = 1 / tan(25.35) = 2.11 					 				(Axis=2.2355; LifeCam=2.11)
//
//					Target Angle = atan(x, y)
//								 = atan(x, 2.11)
//
// Target Distance:	Image Inch width = (Target inch width / Target pixel width) * Image pixel width
//							 		 = (14.62 /Target pixel width) * 320
//							   		 = 4678 / Target pixel width
//					Distance = (Image inch width / 2) / Tan(Camera image angle / 2)
//                           = ((4678 / Target pixel width) / 2) / Tan(25.35)
//							 = (2339 / Tan(25.35)) / Target pixel width
//							 = 4937 / Target pixel width

Vision::Vision() {
    m_task = nullptr;
    m_searchState = ssDone;
    m_targetAngle = 0;
    m_targetDistance = 0;
}

void Vision::FindTarget(TargetSelect targetSelect) {
    Robot::m_robotLog.Write("Vision:  Initiate Target search");
    m_targetAngle = 0;                                                      // Initialize variables
    m_targetDistance = 0;
    m_searchState = ssLooking;

    m_task = new std::thread(TargetSearch, this, targetSelect);             // Start TargetSearch thread
    m_task->detach();
}

bool Vision::GetCameraImage(cv::Mat& image) {
    if(m_sink.GrabFrame(image) == 0) return false;                          // Get image from camera
    return true;
}

Vision::SearchState Vision::GetSearchState() {
    return m_searchState;
}

double Vision::GetTargetAngle() {
    return m_targetAngle;
}

double Vision::GetTargetDistance() {
    return m_targetDistance;
}

void Vision::InitVision() {
    frc::CameraServer* cameraServer = frc::CameraServer::GetInstance();     // Get instance of Camera Server
    m_camera = cameraServer->StartAutomaticCapture("aimCamera", 0);         // Start Aim camera image capture
    m_camera.SetResolution(320, 240);                                       // Configure camera
    m_camera.SetFPS(20);
    m_camera.SetBrightness(20);
    m_camera.SetExposureManual(10);

    m_sink = cameraServer->GetVideo("aimCamera");                           // Create sink to get images for Target Search
    
    m_server.SetSource(m_camera);
    m_server = cameraServer->GetServer();
}

void Vision::SearchResults(bool targetFound, double targetAngle, double targetDistance) {
    if(targetFound) {                                                       // Post search results to Robot thread
        m_searchState = ssTargetFound;
        m_targetAngle = targetAngle;
        m_targetDistance = targetDistance;

        sprintf(m_buffer, "Vision:  Target Angle=%5.1f  Distance=%5.0f", m_targetAngle, m_targetDistance);
        Robot::m_robotLog.Write(m_buffer);
    } else {
        m_searchState = ssNoTarget;
        Robot::m_robotLog.Write("Vision:  No Target Found");
    }
}

 void Vision::TargetSearch(Vision* host, Vision::TargetSelect targetSelect) {     
    struct visionTape {                                                     // Structure containing Vision Tape info
         float  angle;                                                      // Angle of rotated rectangle
         int    x;                                                          // X coordinate of bounding rectangle
         int    width;                                                      // Width of bounding rectangle
     };
     struct visionTarget {                                                  // Structure containing Vision Target info
         double angle;                                                      // Angle of target from center of image
         double distance;                                                   // Distance from target
     };

     double     targetAngle = 0;                                            // Initialize results
     double     targetDistance = 0;    
     bool       targetFound = false;                                
     cv::Mat    imageA;                                                     // Create Mat to store image

    if(host->GetCameraImage(imageA)) {
        cv::Mat imageB;                                                     // Create 2nd Mat to store converted image
        cv::cvtColor(imageA, imageB, cv::COLOR_BGR2HSV);                    // Change image to HSV formate
                                                                            // Filter image using HSV ranges
        cv::inRange(imageB, cv::Scalar(0, 100, 130), cv::Scalar(180, 255, 255), imageA);

        std::vector<std::vector<cv::Point>> contours;                       // Create vector of Contours
        std::vector<cv::Vec4i> hierarchy;
        contours.clear(); 
                                                                            // Find contours in image
        cv::findContours(imageA, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

        imageA.deallocate();                                                // Free memory used by images
        imageB.deallocate();

        printf("Contours Found=%i\n", contours.size());
    
        if (contours.size() > 0) {                                          // Contours found
            std::vector<visionTape> tapes;                                  // Create vector of Vision Tapes
            tapes.clear();

            for (std::vector<cv::Point> contour: contours){                 // Iterate thru contours looking for tapes
                cv::RotatedRect rotatedRect = cv::minAreaRect(contour);     // Draw minimum area rectangle around contour
                float rectAngle = rotatedRect.angle;                        // Get angle of rotated rectangle
                if(rectAngle < -45) rectAngle += 90;                        // Make angle relative to vertical rectangle  

                if (fabs(rectAngle) > 5 && fabs(rectAngle) < 25) {          // Filter angles within 10 degrees of +-15 degrees
                    cv::Rect rect = cv::boundingRect(contour);              // Draw bounding rectangle around contour
                    
                    if(rect.width >= 10 && rect.height >=20) {              // Filter by size of bounding rectangle
                        visionTape tape;                                    // Create new Vision Tape element
                        tape.angle  = rectAngle;                                
                        tape.x = rect.x;
                        tape.width = rect.width;

                        int i = 0;                                          // Find insertion point in Tapes vector
                        for(i = 0; i < tapes.size(); i++){                  //   (Tapes arranged from left-to-right) 
                            if(tapes[i].x > tape.x) break;
                        }

                        std::vector<visionTape>::iterator it;               // Insert tape in Tapes vector
                        it = tapes.begin() + i;
                        tapes.insert(it, tape);
                    }
                }
            }

            printf("Tapes Found=%i\n", tapes.size());

            if (tapes.size() > 0) {                                         // Tapes Found
                std::vector<visionTarget> targets;                          // Create vector of Vision Targets
                targets.clear();
                int leftTape = -2;

                for (int i = 0; i < tapes.size(); i++) {                    // Iterate thru tapes looking for Targets
                    printf("Tape %i: Angle %f rectX=%i rectWidth= %i\n", i, tapes[i].angle,
                            tapes[i].x, tapes[i].width);
                    
                    if (tapes[i].angle > 0){                                // Based on angle, tape is a left tape
                        leftTape = i;
                    } else if (leftTape == i - 1){                          // Current Tape and previous Tape are a Target pair
                        printf("Target Found: Tapes %i and %i\n", i - 1, i);
                                                                            // Define bounding rectangle around Target
                        int     targetWidth = tapes[i].x - tapes[i - 1].x + tapes[i].width;
                        double  targetCenter = (((double)targetWidth / 2) + (double)tapes[i - 1].x - 160) / 160;

                        visionTarget target;
                        target.angle = atan2(targetCenter, 2.11) * 180 / 3.14159;
                        target.distance = 4937 / (double)targetWidth;
            
                        targets.push_back(target);
                    }
                }

                printf("Targets Found=%i\n", targets.size());

                if(targets.size() > 0) {
                    int targetIndex = 0;                                    // Index 0 is leftmost Target

                    if(targetSelect == Vision::tsBest){                     // Search for Target closest to center of Image
                        for (int i = 1; i < targets.size(); i++) {
                            if (fabs(targets[i].angle) < fabs(targets[targetIndex].angle)) targetIndex = i;
                            printf("Vision Target %i: Angle=%f Distance=%f\n", i, targets[i].angle, targets[i].distance);
                        }

                    } else if (targetSelect == Vision::tsRight){
                        targetIndex = targets.size() - 1;
                    }

                    printf("Selected Target: %i\n", targetIndex);
                    targetFound = true;
                    targetAngle = targets[targetIndex].angle;
                    targetDistance = targets[targetIndex].distance;
                }
            }
        }

    } else {                                                                // No image from Camera
        imageA.deallocate();
        Robot::m_robotLog.Write("Vision:  No Image from Camera");
    }

    host->SearchResults(targetFound, targetAngle, targetDistance);
 }

