#ifndef UTIL_H
#define UTIL_H

#define byte unsigned char

#include "sys/time.h"
#include "vector"
#include "iostream"
#include "string"
#include "map"
#include "../common/sockets.h"
//#include "../common/mtcnn.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "../common/frameUnit.h"

using namespace std;

int max_size = 60 * 1024;
#define MAX_SIZE max_size

#define MAXFACEOPEN 0

struct dataUnit
{
    int32_t dataLen;
    byte *dataBytes;

    dataUnit()
    {
    }
    dataUnit(int32_t data_len, byte *data_bytes)
    {
        dataLen = data_len;
        dataBytes = data_bytes;
    }
};

int32_t bytesto_int4(byte *bytes)
{
    //turn bytes array to integer
    int32_t num = bytes[0] & 0xFF;
    num |= ((bytes[1] << 8) & 0xFF00);
    num |= ((bytes[2] << 16) & 0xFF0000);
    num |= ((bytes[3] << 24) & 0xFF000000);
    return num;
}

void int4to_bytes(int32_t num, byte *bytes /*in array size must be 4*/)
{
    //turn integer to bytes array
    bytes[0] = num;
    bytes[1] = num >> 8;
    bytes[2] = num >> 16;
    bytes[3] = num >> 24;
}

void int8to_bytes(int64_t num, byte *bytes /*in array size must be 8*/)
{
    bytes[0] = num;
    bytes[1] = num >> 8;
    bytes[2] = num >> 16;
    bytes[3] = num >> 24;
    bytes[4] = num >> 32;
    bytes[5] = num >> 40;
    bytes[6] = num >> 48;
    bytes[7] = num >> 56;
}

int64_t bytesto_int8(byte *bytes /*size must be 8*/)
{
    //turn bytes array to integer
    int64_t num = bytes[0] & 0xFF;
    num |= (((int64_t)bytes[1] << 8) & 0xFF00);
    num |= (((int64_t)bytes[2] << 16) & 0xFF0000);
    num |= (((int64_t)bytes[3] << 24) & 0xFF000000);
    num |= (((int64_t)bytes[4] << 32) & 0xFF00000000);
    num |= (((int64_t)bytes[5] << 40) & 0xFF0000000000);
    num |= (((int64_t)bytes[6] << 48) & 0xFF000000000000);
    num |= (((int64_t)bytes[7] << 56) & 0xFF00000000000000);
    return num;
}

void arrayCopy(byte *sourse, int sourse_begin, byte *dest, int dest_begin, int len)
{
    //if para error return
    if (sourse + sourse_begin + len - 1 == NULL || dest + dest_begin + len - 1 == NULL)
    {
        printf("arrayCopy() error\n");
        return;
    }

    for (int i = 0; i < len; i++)
    {
        dest[dest_begin + i] = sourse[sourse_begin + i];
    }
    //memcpy(dest+dest_begin,sourse,len);
}

//write data into buffer
void writeDataIntoBUf(byte *buf, int32_t buf_begin, byte *data)
{
    arrayCopy(data, 0, buf, buf_begin, strlen((char *)data));
}

void writeDataIntoBUf(byte *buf, int32_t buf_begin, int32_t data)
{
    byte dataBytes[4];
    int4to_bytes(data, dataBytes);
    arrayCopy(dataBytes, 0, buf, buf_begin, 4);
}

int64_t getCrrTime()
{
    timeval tv;
    gettimeofday(&tv, NULL);
    //record the time frame
    int64_t timeFrame = (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;
    return timeFrame;
}

char *getPeerSockAddrInfo(char* addrInfo,int peerFd)
{
    sockaddr_in peerSockAddr;
    socklen_t peerSockLen = sizeof(peerSockAddr);
    getpeername(peerFd, (sockaddr *)&peerSockAddr, &peerSockLen);
    char *peerIP = inet_ntoa(peerSockAddr.sin_addr);
    int peerPort = peerSockAddr.sin_port;
    sprintf(addrInfo,"%s:%d",peerIP,peerPort);
    return addrInfo;
}

//split frame and return unit set
vector<frameUnit> getFrameUnitSet(cv::Mat frame)
{
    vector<frameUnit> unitSet;
    //encode frame data and store in array
    vector<byte> frame_data;
    imencode(".jpg", frame, frame_data);
    string str_encode(frame_data.begin(), frame_data.end());

    //count splite nums
    int32_t count = (frame_data.size() / (int)MAX_SIZE) + 1;
    int64_t time = getCrrTime();
    //put in the set
    for (int order = 0; order < count; order++)
    {
        frameUnit unit;
        unit.setFrameTime(time);
        unit.setFrameCount(count);
        unit.setFrameOrder(order);
        vector<byte> data;
        if (order == count - 1) //if end
        {
            data.assign(frame_data.begin() + order * MAX_SIZE, frame_data.end());
            if (frame_data.size() % MAX_SIZE == 0) //if splited totally
            {
                unit.setFrameDataLen(MAX_SIZE);
            }
            else //if have data left
            {
                unit.setFrameDataLen(frame_data.size() % MAX_SIZE);
            }
        }
        else //if not end
        {
            data.assign(frame_data.begin() + order * MAX_SIZE, frame_data.begin() + (order + 1) * MAX_SIZE);
            unit.setFrameDataLen(MAX_SIZE);
        }
        unit.setFrameData(data);
        unitSet.push_back(unit);
    }
    return unitSet;
}

void getFrameData(map<int32_t, dataUnit> &frameDataMap, int32_t count, byte *frameData /*in*/)
{
    int index = 0;
    for (int i = 0; i < count; i++)
    {
        //get data unit from map
        dataUnit data_unit = frameDataMap[i];
        byte *data = data_unit.dataBytes;
        //copy in the array
        arrayCopy(data, 0, frameData, index, data_unit.dataLen);
        index += data_unit.dataLen;
        delete[] data;
    }
}

#endif
