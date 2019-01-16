
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

void Vision::FindTarget() {
    m_targetAngle = 0;                                                      // Initialize variables
    m_targetDistance = 0;
    m_searchState = ssLooking;

    m_task = new std::thread(TargetSearch, this);                           // Start TargetSearch thread
    m_task->detach();
}

bool Vision::GetCameraImage(cv::Mat& image) {
    if(m_sink.GrabFrame(image) == 0) return false;                          // Get image from camera
    return true;
}

Vision::SearchState Vision::GetSearchState() {
    return m_searchState;
}

double Vision::GetTargetDistance() {
    return m_targetDistance;
}

double Vision::GetTargetAngle() {
    return m_targetAngle;
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

        sprintf(m_log, "Vision: Target Angle=%5.1f  Distance=%5.0f", m_targetAngle, m_targetDistance);
    } else {
        m_searchState = ssNoTarget;
    }
}

 void Vision::TargetSearch(Vision* host) {                                  // TARGET SEARCH THREAD
     struct visionTape {                                                    // Structure containing Vision Tape info
         float  angle;                                                      // Angle of rotated rectangle
         int    x;                                                          // X coordinate of bounding rectangle
         int    height;                                                     // Height of bounding rectangle
         int    width;                                                      // Width of bounding rectangle
     };
     struct visionTarget {                                                  // Structure containing Vision Target info
         int    area;                                                       // Area of bounding rectangle around target
         double angle;                                                      // Angle of target from center of image
         double distance;                                                   // Distance from target
     };

     cv::Mat imageA;

    if(host->GetCameraImage(imageA)) {
        cv::Mat imageB;
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
    
        std::vector<visionTape> tapes;                                      // Create vector of Vision Tapes
        tapes.clear();

        for (std::vector<cv::Point> contour: contours){                     // Iterate thru contours
            cv::RotatedRect rotatedRect = cv::minAreaRect(contour);         // Draw minimum area rectangle around contour
            float rectAngle = rotatedRect.angle;                            // Get angle of rotated rectangle
            if(rectAngle < -45) rectAngle += 90;                            // Make angle relative to vertical rectangle  

            if (fabs(rectAngle) > 5 && fabs(rectAngle) < 25) {              // Filter angles within 10 degrees of +-15 degrees
                cv::Rect rect = cv::boundingRect(contour);                  // Draw bounding rectangle around contour
                
                if(rect.width >= 10 && rect.height >=20) {                  // Filter by size of bounding rectangle
                    visionTape tape;                                        // Create new Vision Tape element
                    tape.angle  = rectAngle;                                
                    tape.x = rect.x;
                    tape.height = rect.height;
                    tape.width = rect.width;

                    int i = 0;                                              // Find insertion point in Tapes vector
                    for(i = 0; i < tapes.size(); i++){                      //   (Tapes arranged from left-to-right) 
                        if(tapes[i].x > tape.x) break;
                    }

                    std::vector<visionTape>::iterator it;                   // Insert tape in Tapes vector
                    it = tapes.begin() + i;
                    tapes.insert(it, tape);
                }
            }
        }

        std::vector<visionTarget> targets;                                  // Create vector of Vision Targets
        targets.clear();
        int leftTape = -2;

        for (int i = 0; i < tapes.size(); i++) {                            // Iterate thru tapes looking Target pairs
            printf("Tape %i: Angle %f rectX=%i rectHeight=%i rectWidth= %i\n", i, tapes[i].angle,
                     tapes[i].x, tapes[i].height, tapes[i].width);
            
            if (tapes[i].angle > 0){                                        // Based on angle, tape is a left tape
                leftTape = i;
            } else if (leftTape == i - 1){                                  // Current Tape and previous Tape are a Target pair
                printf("Target Found: Tapes %i and %i\n", i - 1, i);
                                                                            // Define bounding rectangle around Target
                int     targetHeight = std::max(tapes[i-1].height, tapes[i].height);
                int     targetWidth = tapes[i].x - tapes[i - 1].x + tapes[i].width;
                double  targetCenter = (((double)targetWidth / 2) + (double)tapes[i - 1].x - 160) / 160;

            }
        }

    } else {                                                                // No image from Camera
        imageA.deallocate();
        host->SearchResults(false, 0, 0);
    }
 }