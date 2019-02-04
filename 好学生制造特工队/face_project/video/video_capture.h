#ifndef VIDEO_CAPTURE_H
#define VIDEO_CAPTURE_H

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "video_frame.h"
#include "sdk/HCNetSDK.h"
#include "string"

class CameraCapture
{
    public:
        //open web camera
        CameraCapture(const std::string url,const int _scale);
        //open local camera
        explicit CameraCapture(int index);

        std::string getCameraUrl() const;

        VideoFrame getFrameFromCamera();

        bool initHIKVision();

        LONG getUserID() const;
    private:
        cv::VideoCapture capture;  
        //rtsp url  
        std::string webCameraUrl;
        //control handle
        LONG lUserID;

        float scale;
};

#endif