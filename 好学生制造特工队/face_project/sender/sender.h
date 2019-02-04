#ifndef SENDER_H
#define SENDER_H

#include "sender_socket.h"
#include "../detect/detector.h"
#include "../video/video_capture.h"
#include "../util/util.h"
#include "string"
/*
*   this class is to control video
*/

const int MaxBufferSize = 65535;

class PiControler
{
  public:
    //argv[] --->  ip + port
    PiControler(const std::string &serv_ip,  const char* serv_port,const std::string webUrl);
    //clean the process
    ~PiControler();
    
    sender_socket &getSenderSocket();
    CameraCapture &getCameraCapture();
    detector &getDetector();

    void sendBoxsMsgToServer(MESSAGETYPE type,vector<int32_t> &dataList);
    void sendLoinMsgToServer(MESSAGETYPE type);

    void mgsParseFactory(MESSAGETYPE type);
  private:
    sender_socket socket;
    detector _detector;
    CameraCapture capture;
};


#endif