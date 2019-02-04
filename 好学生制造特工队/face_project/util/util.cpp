#include "util.h"
#include <arpa/inet.h>
#include <sys/socket.h>

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


void writeDataIntoBUf(byte *buf, int32_t buf_begin, byte *data,int len)
{
    arrayCopy(data, 0, buf, buf_begin, len);
}

void writeDataIntoBUf(byte *buf, int32_t buf_begin, int32_t data)
{
    byte dataBytes[4];
    int4to_bytes(data, dataBytes);
    arrayCopy(dataBytes, 0, buf, buf_begin, 4);
}

void writeDataIntoBUf(byte *buf, int32_t buf_begin, int64_t data)
{
    byte dataBytes[8];
    int8to_bytes(data, dataBytes);
    arrayCopy(dataBytes, 0, buf, buf_begin, 8);
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