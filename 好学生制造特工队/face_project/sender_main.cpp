#include "./common/message.h"
#include "./sender/sender.h"
#include "./util/util.h"
#include "iostream"
#include "unistd.h"

void *controlThread(void *arg);

//void sendUnquarifiedScene(int &serv_sock, vector<frameUnit> &unitSet, int64_t &timeFrame);

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage : %s <IP> <port> ....\n", argv[0]);
        exit(1);
    }

    std::string serv_ip(argv[1]); //server host ip
    PiControler *control = new PiControler(serv_ip, argv[2],
                                           "rtsp://admin:admin123456@192.168.0.64/Streaming/Channels/2");

    if (!control->getSenderSocket().connectToServer())
    {
        std::cerr << "Could not connect to server." << std::endl;
        return -1;
    }
    std::cout << "connect successfully" << std::endl;

    if (!control->getCameraCapture().initHIKVision())
    {
        std::cerr << "Could not init HIKVision." << std::endl;
        return -1;
    }
    std::cout << "init HIKVision successfully" << std::endl;

    //send logo in message
    control->sendLoinMsgToServer(PI_LOGIN_MESSAGE);
    pthread_t t_id;
    if (pthread_create(&t_id, NULL, controlThread, (void *)control) != 0)
    {
        std::cerr << "thread error" << std::endl;
        return -1;
    }

    while (true)
    {
        byte typeBytes[4];
        read(control->getSenderSocket().getSocketHandle(), typeBytes, 4);
        int type = bytesto_int4(typeBytes);
        std::cout << "recieved type: " << type << std::endl;
        control->mgsParseFactory(type);
    }
    pthread_join(t_id, NULL);
    return 0;
}

void *controlThread(void *arg)
{
    PiControler *control = (PiControler *)arg;
    detector &_detector = control->getDetector();
    CameraCapture capture = control->getCameraCapture();

    int64_t nowTime = getCrrTime();
    int64_t preTime = preTime;

    while (1)
    {
        nowTime = getCrrTime();
        if (nowTime - preTime >= 1000000)
        {
            preTime = nowTime;
            VideoFrame frame = capture.getFrameFromCamera();
            //start detect
            cv::Mat image = frame.getFrameMat();
            vector<int> BoxLocation = _detector.detectAFrame(image);
            if (BoxLocation.size() == 1)
                continue;
            //write box location message
            control->sendBoxsMsgToServer(PI_BOXS_LOCATION_MESSAGE, BoxLocation);
            std::cout << "boxs num: " << BoxLocation[0] << std::endl;
            std::cout << "locations: " << BoxLocation[1] << BoxLocation[2] << BoxLocation[3] << BoxLocation[4] << std::endl;

            //write unquarified scene
            // if (nowFrameTime - preUnquarifiedTime > 12000000 && BoxLocation.size() == 0) //20min
            // {
            //     //1200000000
            //     preUnquarifiedTime = nowFrameTime;
            //     sendUnquarifiedScene(
            //         control.getSenderSocket().getSocketHandle(), frame, nowFrameTime);
            // }
        }
    }
}

// void sendUnquarifiedScene(int serv_sock, VideoFrame &frame, int64_t &timeFrame)
// {

//     int32_t buflen = 4 + 8 + 4 + framedataLen;
//     byte *unquarifiedSendBuf = new byte[buflen];
//     //write message
//     writeDataIntoBUf(unquarifiedSendBuf, 0, PI_UNQUALIFIED_SCENE_MESSAGE);
//     //write time
//     writeDataIntoBUf(unquarifiedSendBuf, 4, timeFrame);
//     //write data len
//     writeDataIntoBUf(unquarifiedSendBuf, 4 + 8, framedataLen);
//     //write data
//     int32_t startIndex = 4 + 8 + 4;
//     for (int i = 0; i < unitSet.size(); i++)
//     {
//         byte *dataPtr = &(*unitSet[i].getFrameData().begin());
//         writeDataIntoBUf(unquarifiedSendBuf, startIndex, dataPtr, unitSet[i].getFrameData().size());
//         startIndex = startIndex + unitSet[i].getFrameDataLen();
//     }
//     write(serv_sock, unquarifiedSendBuf, buflen);

//     delete unquarifiedSendBuf;
// }
