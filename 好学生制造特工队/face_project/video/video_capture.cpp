#include "video_capture.h"
#include "string"
CameraCapture::CameraCapture(const std::string url, const int _scale) : webCameraUrl(url), scale(_scale), capture(url), lUserID(-1)
{
}

VideoFrame CameraCapture::getFrameFromCamera()
{
    if (!capture.isOpened())
    {
        std::cerr << "could not open web camera using url " << webCameraUrl << std::endl;
        exit(-1);
    }

    cv::Mat image;
    capture >> image;
    if (scale < 1.0)
    {
        cv::resize(image, image, cv::Size(0, 0), scale, scale);
    }

    VideoFrame frame(image);
    return frame;
}

std::string CameraCapture::getCameraUrl() const
{
    return webCameraUrl;
}

bool CameraCapture::initHIKVision()
{
    //init
    NET_DVR_Init();
    //set connect time
    NET_DVR_SetConnectTime(2000, 1);
    //set reconnect time
    NET_DVR_SetReconnect(10000, true);
    //--------------------------------------
    //register device

    NET_DVR_DEVICEINFO_V30 deviceInfo;
    //logo in
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    NET_DVR_DEVICEINFO_V40 struDeviceInfo = {0};
    strcpy((char *)struLoginInfo.sDeviceAddress, "192.168.0.64"); //device ip
    strcpy((char *)struLoginInfo.sUserName,"admin"); //user
    strcpy((char *)struLoginInfo.sPassword,"admin123456"); //password 
    struLoginInfo.wPort = 8000;
    struLoginInfo.bUseAsynLogin = 0; //synchronized login
    lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfo);
    if (lUserID < 0)
    {
        printf("NET_DVR_Login_V40 failed, error code: %d\n", NET_DVR_GetLastError());
        NET_DVR_Cleanup();
        return false;
    }
    return true;
}

LONG CameraCapture::getUserID() const
{
    return lUserID;
}
