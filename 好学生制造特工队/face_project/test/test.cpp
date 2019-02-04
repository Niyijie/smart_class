#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "iostream"
#include <stdio.h>
#include "HCNetSDK.h"
#include <time.h>
#include <unistd.h>
#include "string.h"

using namespace std;
using namespace cv;

void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    char tempbuf[256] = {0};
    switch (dwType)
    {
    case EXCEPTION_RECONNECT: //预览时重连
        printf("----------reconnect--------%d\n", (int)time(NULL));
        break;
    default:
        break;
    }
}

int main()
{
    //--------------------------------------- //初始化
    NET_DVR_Init();
    //设置连接时间与重连时间
    NET_DVR_SetConnectTime(2000, 1);
    NET_DVR_SetReconnect(10000, true);
    //---------------------------------------
    //设置异常消息回调函数
    NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);
    //--------------------------------------- // 注册设备
    LONG lUserID = -1;


    NET_DVR_DEVICEINFO_V30 deviceInfo;
    lUserID = NET_DVR_Login_V30("192.168.0.64", 8000, "user01", "admin123456", &deviceInfo);
    if (lUserID < 0)
    {
        printf("NET_DVR_Login_V40 failed, error code: %d\n", NET_DVR_GetLastError());
        NET_DVR_Cleanup();
        return 0;
    }
    //云台控制:云台左转
    if (!NET_DVR_PTZControl_Other(lUserID, 1, PAN_LEFT, 0)) //开始左转
    {
        printf("NET_DVR_PTZControl_Other left failed, error code: %d\n", NET_DVR_GetLastError());
        NET_DVR_Cleanup();
        return 0;
    }
    printf("云台开始左转!\n");
    sleep(5000);                                            //转动 5s
    if (!NET_DVR_PTZControl_Other(lUserID, 1, PAN_LEFT, 1)) //停止左转
    {
        printf("NET_DVR_PTZControl_Other failed, error code: %d\n", NET_DVR_GetLastError());
        NET_DVR_Cleanup();
        return 0;
    }
    printf("云台左转停止!\n");
    NET_DVR_Logout(lUserID);
    //释放 SDK 资源
    NET_DVR_Cleanup();
    return 0;
}

int main1()
{
    //control();
    VideoCapture capture("rtsp://admin:admin123456@192.168.0.64/Streaming/Channels/1");
    cout << "视频相关信息如下" << endl;
    cout << "宽度：" << capture.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
    cout << "高度：" << capture.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;

    cout << "总帧数：" << capture.get(CV_CAP_PROP_FRAME_COUNT) << endl;
    cout << "帧率（fps）：" << capture.get(CV_CAP_PROP_FPS) << endl;
    if (!capture.isOpened())
    {
        printf("can't open camera...\n");
        return -1;
    }
    cout << "opened" << endl;
    namedWindow("window", CV_WINDOW_NORMAL);
    resizeWindow("window", 800, 800);
    int i = 0;
    const std::vector<int> compression_params = {cv::IMWRITE_JPEG_QUALITY, 25};

    while (true)
    {
        Mat frame;
        capture >> frame;
        imshow("window", frame);
        char c = waitKey(20);
        if (c == 27)
            break;
    }
}
