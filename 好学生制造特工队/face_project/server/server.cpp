#include "map"
#include "string"
#include "server.h"
#include "../util/util.h"
#include "unistd.h"

ProcessServer::ProcessServer(const char *serv_port) : socket(serv_port)
{
}

server_socket &ProcessServer::getServerSocket()
{
    return socket;
}

std::map<std::string, piInfo> &ProcessServer::getPiSocketMap()
{
    return piSocketMap;
}

int ProcessServer::getClientSocketHandle() const
{
    return clientSocketHandle;
}

void ProcessServer::mgsParseFactory(MESSAGETYPE type, int fd)
{
    switch (type)
    {
    case PI_LOGIN_MESSAGE:
    {
        /*
        *  write to pi
        */
        char addrInfo[100];
        //get peer sock addr info
        getPeerSockAddrInfo(addrInfo, fd);

        //get rtsp addr
        byte addrLenBytes[4];
        read(fd, addrLenBytes, 4);
        int addrLen = bytesto_int4(addrLenBytes);
        byte *addr = new byte[addrLen];
        read(fd, addr, addrLen);
        std::string rtspAddr((char *)addr);
        piInfo info(rtspAddr, fd);
        //put it into map
        std::string piName(addrInfo);
        std::cout << "pi login " << addrInfo << "piName " << piName << std::endl;
        piSocketMap.insert(make_pair(piName, info));
        delete addr;
        break;
    }
    case CLIENT_LOGIN_MESSAGE:
    {
        clientSocketHandle = fd;
        break;
    }
    case PI_BOXS_LOCATION_MESSAGE:
    {
        //get boxs nums
        byte numsBytes[4];
        read(fd, numsBytes, 4);
        int32_t BoxsNums = bytesto_int4(numsBytes);

        char addrInfo[100];
        //get peer sock addr info
        getPeerSockAddrInfo(addrInfo, fd);
        //buffer
        int32_t bufLen = 4 + 4 + strlen(addrInfo) + 4 + BoxsNums * 4 * 4;
        byte *buffer = new byte[bufLen];
        //write message
        writeDataIntoBUf(buffer, 0, PI_BOXS_LOCATION_MESSAGE);
        //write addr info len
        writeDataIntoBUf(buffer, 4, (int32_t)strlen(addrInfo));
        //write addr info
        writeDataIntoBUf(buffer, 8, (byte *)addrInfo, strlen(addrInfo));
        //write box num
        writeDataIntoBUf(buffer, 8 + strlen(addrInfo), BoxsNums);
        read(fd, buffer + 8 + strlen(addrInfo) + 4, BoxsNums * 4 * 4);
        //count buffer len
        if (clientSocketHandle != -1)
            write(clientSocketHandle, buffer, bufLen);
        delete buffer;
        break;
    }
    case CLIENT_RTSP_GET_MESSAGE:
    {
        if (piSocketMap.size() == 0)
            break;

        //count buffer len
        int bufferLen = 0;
        std::map<std::string, piInfo>::iterator iter;
        for (iter = piSocketMap.begin(); iter != piSocketMap.end(); iter++)
        {
            bufferLen += iter->first.size();
            bufferLen += iter->second.RTSPAdrr.size();
        }

        //write info into buffer
        byte *buffer = new byte[4 + 4 + 4 * piSocketMap.size() + 4 * piSocketMap.size() + bufferLen];
        writeDataIntoBUf(buffer, 0, PI_RTSPS_MESSAGE);
        writeDataIntoBUf(buffer, 4, (int32_t)piSocketMap.size());
        int index = 8;
        for (iter = piSocketMap.begin(); iter != piSocketMap.end(); iter++)
        {
            //write ip addr
            writeDataIntoBUf(buffer, index, (int32_t)iter->first.size());
            std::string piName = iter->first;
            std::cout << "client rtsp login " << piName << std::endl;
            writeDataIntoBUf(buffer, index + 4, (byte *)piName.c_str(), piName.size());
            //write rtsp addr
            std::string addr = iter->second.RTSPAdrr;
            writeDataIntoBUf(buffer, index + 4 + iter->first.size(), (int32_t)addr.size());
            writeDataIntoBUf(buffer, index + 4 + iter->first.size() + 4, (byte *)addr.c_str(), addr.size());
            index = index + 4 + iter->first.size() + 4 + addr.size();
        }
        //write to client
        write(fd, buffer, index);

        delete buffer;
        break;
    }
    case PI_UNQUALIFIED_SCENE_MESSAGE:
    {
        // //get timeframe
        // byte timeFrameBytes[8];
        // read(readfd, timeFrameBytes, 8);
        // //get datalen
        // byte datalenBytes[4];
        // read(readfd, datalenBytes, 4);
        // int32_t len = bytesto_int4(datalenBytes);
        // //get peer sock addr info
        // char addrInfo[100];
        // getPeerSockAddrInfo(addrInfo, readfd);
        // //send buf
        // byte *unquaryfiedBuf = new byte[4 + 4 + strlen(addrInfo) + 8 + 4];
        // //write message
        // writeDataIntoBUf(unquaryfiedBuf, 0, PI_UNQUALIFIED_SCENE_MESSAGE);
        // //write addr info len
        // writeDataIntoBUf(unquaryfiedBuf, 4, (int32_t)strlen(addrInfo));
        // //write addr info
        // writeDataIntoBUf(unquaryfiedBuf, 4 + 4, (byte *)addrInfo, strlen(addrInfo));
        // //write frame time into buf
        // writeDataIntoBUf(unquaryfiedBuf, 4 + 4 + strlen(addrInfo), timeFrameBytes, 8);
        // //write data len into buf
        // writeDataIntoBUf(unquaryfiedBuf, 4 + 4 + strlen(addrInfo) + 8, datalenBytes, 4);
        // //write to client
        // write(client_sock, unquaryfiedBuf, 4 + 4 + strlen(addrInfo) + 8 + 4);
        // //write data to client
        // byte sendbuf[60 * 1024];
        // while (len > 0)
        // {
        //     //each sent max datalen is 60kb
        //     int sentLen = (len > 60 * 1024) ? (60 * 1024) : len;
        //     int getLen = read(readfd, sendbuf, sentLen);
        //     write(client_sock, sendbuf, getLen);
        //     bzero(sendbuf, sizeof(sendbuf));
        //     len = len - getLen;
        // }
        // delete unquaryfiedBuf;
        // break;
    }
    case CLIENT_LOGOFF_MESSAGE:
    {
        close(clientSocketHandle);
        clientSocketHandle = -1;
        break;
    }
    case PI_LOGOFF_MESSAGE:
    {
        char addrInfo[100];
        //get peer sock addr info
        getPeerSockAddrInfo(addrInfo, fd);
        //put it into map
        std::string piName(addrInfo);
        piSocketMap.erase(piName);
        close(fd);
        break;
    }
    case CLIENT_CONTROL_MESSAGE:
    {
        byte addrBytesLen[4];
        //read addr len
        read(clientSocketHandle, addrBytesLen, 4);
        //read addr
        int32_t addrLen = bytesto_int4(addrBytesLen);
        byte *addr = new byte[addrLen];
        read(clientSocketHandle, addr, addrLen);
        //get pi sock fd
        std::string piName((char *)addr, addrLen);

        // std::map<std::string, piInfo>::iterator iter;
        // std::cout << "size: " << piSocketMap.size() << std::endl;
        // for (iter = piSocketMap.begin(); iter != piSocketMap.end(); iter++)
        // {
        //     std::cout << "name: " << iter->first << std::endl;
        //     std::cout << "fd: " << iter->second.fd << std::endl;
        // }
        // std::cout << "ending......" << std::endl;

        if (piSocketMap.find(piName) != piSocketMap.end())
        {
            int sockfd = piSocketMap[piName].fd;
            std::cout << "piname :" << piName << " fd: " << sockfd << std::endl;
            //get detail control type
            byte msgType[4];
            read(clientSocketHandle, msgType, 4);
            write(sockfd, msgType, 4);
        }
        delete addr;
        break;
    }
    }
}