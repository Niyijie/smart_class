#include "../common/sockets.h"
#include "../common/util.h"
#include "../common/frameUnit.h"
#include "map"

using namespace std;

int main(int argc,char *argv[])
{
    if(argc != 2)
    {
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }
    //connect setting
    sockaddr_in serv_addr,clnt_addr;
    int serv_sock = Socket(PF_INET,SOCK_DGRAM,0);
    addr_init(&serv_addr,AF_INET,INADDR_ANY,argv[1]);
    Bind(serv_sock,&serv_addr,sizeof(serv_addr));

    //init value
    byte packet[8 + 4 + 4 + 4 + MAX_SIZE];
    socklen_t clnt_adr_len;
    int64_t preTime = 0;
    map<int32_t,dataUnit> frameDataMap;
    int32_t count = 0;
    int32_t dataTotalLen = 0;
    while(true)
    {
        size_t  packetLen = recvfrom(serv_sock,packet,sizeof(packet),0,(sockaddr*)&clnt_addr,&clnt_adr_len);
        //get time
        byte timeBytes[8];
        arrayCopy(packet,0,timeBytes,0,8);
        int64_t frameTime = bytesto_int8(timeBytes);
        if(frameTime < preTime)
        {
            continue;
        }
        else
        {
            if(frameTime > preTime)
            {
                //if new frame arrived
                preTime = frameTime;
                frameDataMap.clear();
                dataTotalLen = 0;
                //get count
                byte countBytes[4];
                arrayCopy(packet,8,countBytes,0,4);
                count = bytesto_int4(countBytes);
            }
            //get order
            byte orderBytes[4];
            arrayCopy(packet,12,orderBytes,0,4);
            int32_t order = bytesto_int4(orderBytes);
            //get data len
            byte dataLenBytes[4];
            arrayCopy(packet,16,dataLenBytes,0,4);
            int32_t datalen = bytesto_int4(dataLenBytes);
            dataTotalLen += datalen;
            //get data
            byte* dataBytes = new byte[datalen];
            bzero(dataBytes,datalen);
            arrayCopy(packet,20,dataBytes,0,datalen);
            //add in the map
            frameDataMap.insert(pair<int32_t, dataUnit>(order, dataUnit(datalen,dataBytes)));
            dataUnit dataBytes1 = frameDataMap[order];
            //if frame packets is all arrived and convert it to a picture
            if(frameDataMap.size() == count)
            {
                byte* frameData = new byte[dataTotalLen];
                getFrameData(frameDataMap,count,frameData);
  
                Mat frame;
                vector<byte> d(frameData,frameData+dataTotalLen);
                frame = imdecode(Mat(d), CV_LOAD_IMAGE_COLOR);
                imshow("picture",frame);
                delete[] frameData;
                char t = waitKey(30);
                if(t == 27)
                {
                    break;
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}   