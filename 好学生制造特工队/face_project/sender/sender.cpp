#include "sender.h"
#include "string"
#include "vector"
#include "../util/util.h"
#include "../video/sdk/HCNetSDK.h"
#include "unistd.h"

PiControler::PiControler(
    const std::string &serv_ip, const char *serv_port, const std::string webUrl) : socket(serv_ip, serv_port), capture(webUrl, 1), _detector("./detect/models/")
{
}

PiControler::~PiControler()
{
    //close connect
    close(socket.getSocketHandle());
}

sender_socket &PiControler::getSenderSocket()
{
    return socket;
}

CameraCapture &PiControler::getCameraCapture()
{
    return capture;
}

detector &PiControler::getDetector()
{
    return _detector;
}

void PiControler::sendBoxsMsgToServer(MESSAGETYPE type, vector<int32_t> &dataList)
{
    int32_t size = dataList.size();
    if (size == 0 || type != PI_BOXS_LOCATION_MESSAGE)
        return;
    byte *writeBuffer = new byte[4 * (size + 1)];
    writeDataIntoBUf(writeBuffer, 0, type);
    for (int i = 0; i < dataList.size(); i++)
    {
        writeDataIntoBUf(writeBuffer, (i + 1) * 4, dataList[i]);
    }
    write(socket.getSocketHandle(), writeBuffer, (size + 1) * 4);
    delete writeBuffer;
}

void PiControler::sendLoinMsgToServer(MESSAGETYPE type)
{
    if (type != PI_LOGIN_MESSAGE)
        return;
    //send login message
    std::string url = getCameraCapture().getCameraUrl();
    byte *writeBuffer = new byte[4 + 4 + url.size()];
    //msg type
    writeDataIntoBUf(writeBuffer, 0, PI_LOGIN_MESSAGE);
    //data len
    writeDataIntoBUf(writeBuffer, 4, (int32_t)url.size());
    //url msg data
    writeDataIntoBUf(writeBuffer, 8, (byte *)url.c_str(), (int32_t)url.size());
    //write to server
    write(socket.getSocketHandle(), writeBuffer, 4 + 4 + url.size());
    delete writeBuffer;
}

void PiControler::mgsParseFactory(MESSAGETYPE type)
{
    switch (type)
    {
    case CLIENT_PAN_LEFT_MESSAGE:
    {
        //start turn left
        if (!NET_DVR_PTZControl_Other(capture.getUserID(), 1, PAN_LEFT, 0))
        {
            printf("NET_DVR_PTZControl_Other left failed, error code: %d\n", NET_DVR_GetLastError());
            NET_DVR_Cleanup();
            break;
        }
        printf("start turn left!\n");
        break;
    }
    case CLIENT_LEFT_STOP_MESSAGE:
    {
        //start turn left
        if (!NET_DVR_PTZControl_Other(capture.getUserID(), 1, PAN_LEFT, 1))
        {
            printf("NET_DVR_PTZControl_Other left stop failed, error code: %d\n", NET_DVR_GetLastError());
            NET_DVR_Cleanup();
            break;
        }
        printf("turn left stop!\n");
        break;
    }
    case CLIENT_PAN_RIGHT_MESSAGE:
    {
        //start turn left
        if (!NET_DVR_PTZControl_Other(capture.getUserID(), 1, PAN_RIGHT, 0))
        {
            printf("NET_DVR_PTZControl_Other right failed, error code: %d\n", NET_DVR_GetLastError());
            NET_DVR_Cleanup();
            break;
        }
        printf("start turn right!\n");
        break;
    }
    case CLIENT_RIGHT_STOP_MESSAGE:
    {
        //start turn left
        if (!NET_DVR_PTZControl_Other(capture.getUserID(), 1, PAN_RIGHT, 1))
        {
            printf("NET_DVR_PTZControl_Other right stop failed, error code: %d\n", NET_DVR_GetLastError());
            NET_DVR_Cleanup();
            break;
        }
        printf("turn right stop!\n");
        break;
    }
    case CLIENT_ZOOM_IN_MESSAGE:
    {
        //start turn left
        if (!NET_DVR_PTZControl_Other(capture.getUserID(), 1, ZOOM_IN, 0))
        {
            printf("NET_DVR_PTZControl_Other zoom in failed, error code: %d\n", NET_DVR_GetLastError());
            NET_DVR_Cleanup();
            break;
        }
        printf("start zoom in!\n");
        break;
    }
    case CLIENT_ZOOM_IN_STOP_MESSAGE:
    {
        //start turn left
        if (!NET_DVR_PTZControl_Other(capture.getUserID(), 1, ZOOM_IN, 1))
        {
            printf("NET_DVR_PTZControl_Other zoom in stop failed, error code: %d\n", NET_DVR_GetLastError());
            NET_DVR_Cleanup();
            break;
        }
        printf("zoom in stop!\n");
        break;
    }
    case CLIENT_ZOOM_OUT_MESSAGE:
    {
        //start turn left
        if (!NET_DVR_PTZControl_Other(capture.getUserID(), 1, ZOOM_OUT, 0))
        {
            printf("NET_DVR_PTZControl_Other zoom out failed, error code: %d\n", NET_DVR_GetLastError());
            NET_DVR_Cleanup();
            break;
        }
        printf("zoom out!\n");
        break;
    }
    case CLIENT_ZOOM_OUT_STOP_MESSAGE:
    {
        //start turn left
        if (!NET_DVR_PTZControl_Other(capture.getUserID(), 1, ZOOM_OUT, 1))
        {
            printf("NET_DVR_PTZControl_Other zoom out stop failed, error code: %d\n", NET_DVR_GetLastError());
            NET_DVR_Cleanup();
            break;
        }
        printf("zoom out stop!\n");
        break;
    }
    default:
        break;
    }
}
