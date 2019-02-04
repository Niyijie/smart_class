#ifndef UTIL_H
#define UTIL_H

#define byte unsigned char

#include "sys/time.h"
#include "vector"
#include "iostream"
#include "string"
#include "map"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "../common/message.h"

using namespace std;

// int max_size = 100 * 1024;
// #define MAX_SIZE max_size

#define MESSAGETYPE int

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

int32_t bytesto_int4(byte *bytes);

void int4to_bytes(int32_t num, byte *bytes /*in array size must be 4*/);

void int8to_bytes(int64_t num, byte *bytes /*in array size must be 8*/);

int64_t bytesto_int8(byte *bytes /*size must be 8*/);

void arrayCopy(byte *sourse, int32_t sourse_begin, byte *dest, int32_t dest_begin, int32_t len);

void writeDataIntoBUf(byte *buf, int32_t buf_begin, byte *data,int32_t len);

void writeDataIntoBUf(byte *buf, int32_t buf_begin, int32_t data);

void writeDataIntoBUf(byte *buf, int32_t buf_begin, int64_t data);

int64_t getCrrTime();

char *getPeerSockAddrInfo(char* addrInfo,int peerFd);

#endif
