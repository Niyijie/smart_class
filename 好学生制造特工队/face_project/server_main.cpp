#include "sys/epoll.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "map"
#include "queue"
#include "pthread.h"
#include "./util/util.h"
#include "./common/message.h"
#include "./server/server.h"

void *tranSenderData(void *arg);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    //init server
    ProcessServer server(argv[1]);
    bool ret = server.getServerSocket().bindSocketAndListen(50);
    if (ret == false)
    {
        std::cerr << "bind error" << std::endl;
    }

    /*
    * use epoll mode to serve
    */
    int epfd, nfds;
    /*
    * ev for registering events, 
    * and event array for returning events to be processed
    */
    epoll_event ev, events[20];
    //epfd is epoll fd
    epfd = epoll_create(256);
    //relavent sock fd
    int serverSocketHandle = server.getServerSocket().getSocketHandle();
    ev.data.fd = serverSocketHandle;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serverSocketHandle, &ev);
    while (true)
    {
        nfds = epoll_wait(epfd, events, 20, 500);
        for (int i = 0; i < nfds; ++i)
        {
            if (events[i].data.fd == serverSocketHandle) //if connection require
            {
                int sockfd = accept(serverSocketHandle, NULL, 0);
                if (sockfd == -1)
                {
                    std::cerr << "accept error" << std::endl;
                    continue;
                }
                printf("accept new connect success fd: %d", sockfd);
                //register event
                ev.data.fd = sockfd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
            }
            else if (events[i].events & EPOLLIN) //other fd,read it
            {
                int readfd = events[i].data.fd;

                //listen tcp port
                byte typeBytes[4];
                read(readfd, typeBytes, 4);
                MESSAGETYPE type = bytesto_int4(typeBytes);
                std::cout << type << std::endl;
                server.mgsParseFactory(type, readfd);
            }
        }
    }
}

// void *tranSenderData(void *arg)
// {
//     sockInfo info = *((sockInfo *)arg);
//     int serv_trans_sock = info.serv_sock;
//     int client_sock = info.client_sock;

//     cout << "pi accept start" << endl;
//     int pi_sock = accept(serv_trans_sock, NULL, NULL);
//     if (pi_sock == -1)
//     {
//         cout << "tranSenderUdpData() error !!!!" << endl;
//         return 0;
//     }
//     cout << "pi accept successfully" << endl;
//     //write to client to connect
//     char port_c[255];
//     sprintf(port_c, "%d", info.serv_port);

//     byte msgbuf[1024];
//     /*
//     *  write to client
//     */
//     char addrInfo[100];
//     //get peer sock addr info
//     getPeerSockAddrInfo(addrInfo, info.pi_sock);
//     //write message into buf
//     writeDataIntoBUf(msgbuf, 0, SERV_PORT_MESSAGE);
//     //write addr info len
//     writeDataIntoBUf(msgbuf, 4, (int32_t)strlen(addrInfo));
//     //write addr info
//     writeDataIntoBUf(msgbuf, 8, (byte *)addrInfo, strlen(addrInfo));
//     //write ports
//     writeDataIntoBUf(msgbuf, 8 + strlen(addrInfo), (int32_t)info.serv_port);

//     int32_t bufLen = 4 + 4 + strlen(addrInfo) + 4;
//     write(client_sock, msgbuf, bufLen);
//     //client thread get frame data
//     cout << "client accept start" << endl;
//     int client_sock2 = accept(serv_trans_sock, NULL, NULL);
//     cout << "client accept successfully" << endl;
//     if (client_sock2 == -1)
//     {
//         cout << "tranSenderUdpData() error !!!!" << endl;
//         return 0;
//     }
//     //transfer data
//     byte frameBuffer[MAX_SIZE];
//     while (true)
//     {
//         int getlen = read(pi_sock, frameBuffer, MAX_SIZE);
//         write(client_sock2, frameBuffer, getlen);
//         bzero(frameBuffer, MAX_SIZE);
//         cout << "get len : " << getlen << endl;
//     }
//     close(serv_trans_sock);
//     return 0;
// }