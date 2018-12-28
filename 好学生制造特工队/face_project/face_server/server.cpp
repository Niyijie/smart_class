#include "../common/sockets.h"
#include "../common/util.h"
#include "../common/frameUnit.h"
#include "stdlib.h"
#include <stdio.h>
#include <sys/epoll.h>
#include "map"
#include "queue"
#include "pthread.h"
#include "../common/message.h"

using namespace std;

#define CLIENT_IP "172.20.10.3"

void *client_line(void *arg);
void *tranSenderUdpData(void *arg);
//Allocate a port for each connection starting at 5000
int ports = 5000;
//store client udp port
queue<int> queue_port;

byte buf[2048];

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    //tcp server listen
    sockaddr_in local_addr;
    socklen_t local_addr_len;
    int listen_sock = Socket(PF_INET, SOCK_STREAM, 0);
    addr_init(&local_addr, AF_INET, INADDR_ANY, argv[1]);
    Bind(listen_sock, &local_addr, sizeof(local_addr));
    int ret = listen(listen_sock, 5);
    if (ret == -1)
    {
        cout << "listen error" << endl;
        return 0;
    }
    //client login
    sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    int client_sock = accept(listen_sock, NULL, NULL);
    if (client_sock == -1)
    {
        cout << "accept error!!!!" << endl;
        return 0;
    }
    else
    {
        cout << "client login......." << endl;
    }

    pthread_t client_id;
    cout << "client sock " << client_sock << endl;
    if (pthread_create(&client_id, NULL, client_line, (void *)&client_sock) != 0)
    {
        cout << "thread1 error" << endl;
        return -1;
    }
    else
    {
        cout << "client thread start()....." << endl;
    }

    //use epoll mode to serve
    int epfd, nfds;
    /*
    * ev for registering events, 
    * and event array for returning events to be processed
    */
    epoll_event ev, events[20];
    //epfd is epoll fd
    epfd = epoll_create(256);
    //relavent sock fd
    ev.data.fd = listen_sock;
    ev.events = EPOLLIN | EPOLLET;
    //register epoll event
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_sock, &ev);
    epoll_ctl(epfd, EPOLL_CTL_ADD, client_sock, &ev);

    while (true)
    {
        nfds = epoll_wait(epfd, events, 20, 500);
        for (int i = 0; i < nfds; ++i)
        {
            if (events[i].data.fd == listen_sock) //if connection from raspberry pie
            {
                int pi_sock = accept(listen_sock, NULL, 0);
                if (pi_sock == -1)
                {
                    cout << "accept error" << endl;
                }
                else
                {
                    cout << "pi login......." << endl;
                }
                //register event
                ev.data.fd = pi_sock;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_ADD, pi_sock, &ev);
            }
            else if (events[i].events & EPOLLIN) //other fd,read it
            {
                int readfd = events[i].data.fd;

                //listen tcp port
                byte msgBytes[4];
                int dataLen = read(readfd, msgBytes, 4);
                int type = bytesto_int4(msgBytes);
                switch (type)
                {
                case PI_LOGIN_MESSAGE:
                {
                    /*
                    *  write to client
                    */
                    char addrInfo[100];
                    //get peer sock addr info
                    getPeerSockAddrInfo(addrInfo, readfd);
                    //write message into buf
                    writeDataIntoBUf(buf, 0, PI_LOGIN_MESSAGE);
                    //write addr info len
                    writeDataIntoBUf(buf, 4, strlen(addrInfo));
                    //write addr info
                    writeDataIntoBUf(buf, 8, (byte *)addrInfo);
                    int32_t bufLen = 4 + 4 + strlen(addrInfo);
                    write(client_sock, buf, bufLen);

                    /*
                    *  write to pi
                    */
                    char port_s[255]; //int to char
                    sprintf(port_s, "%d", ports);
                    ////init udp server socket
                    sockaddr_in serv_addr;
                    int serv_sock = Socket(PF_INET, SOCK_DGRAM, 0);
                    addr_init(&serv_addr, AF_INET, INADDR_ANY, port_s);
                    Bind(serv_sock, &serv_addr, sizeof(serv_addr));
                    //Tell the raspberry dispatch to the specified port of the server UDP

                    writeDataIntoBUf(buf, 0, SERV_PORT_MESSAGE);
                    writeDataIntoBUf(buf, 4, ports);
                    write(readfd, buf, 8);
                    bzero(buf, sizeof(buf));
                    //Open a threads to process requests
                    pthread_t t_id;
                    if (pthread_create(&t_id, NULL, tranSenderUdpData, (void *)&serv_sock) != 0)
                    {
                        cout << "thread error" << endl;
                        return -1;
                    }
                    //next serv udp port
                    ports++;
                    break;
                }
                case PI_BOXS_MESSAGE:
                {
                    char addrInfo[100];
                    //get peer sock addr info
                    getPeerSockAddrInfo(addrInfo, readfd);
                    //write message
                    writeDataIntoBUf(buf, 0, PI_BOXS_MESSAGE);
                    //write addr info len
                    writeDataIntoBUf(buf, 4, strlen(addrInfo));
                    //write addr info
                    writeDataIntoBUf(buf, 8, (byte *)addrInfo);
                    //write box num into buf
                    read(readfd,buf + 8 + strlen(addrInfo),4);
                    //sent to client
                    write(client_sock, buf, 8 + strlen(addrInfo) + 4);
                    break;
                }
                case PI_BOXS_LOCATION_MESSAGE:
                {
                    byte lenBytes[4];
                    read(readfd, lenBytes, 4);
                    int32_t BoxsDataLen = bytesto_int4(lenBytes);

                    char addrInfo[100];
                    //get peer sock addr info
                    getPeerSockAddrInfo(addrInfo, readfd);
                    //write message
                    writeDataIntoBUf(buf, 0, PI_BOXS_LOCATION_MESSAGE);
                    //write addr info len
                    writeDataIntoBUf(buf, 4, strlen(addrInfo));
                    //write addr info
                    writeDataIntoBUf(buf, 8, (byte *)addrInfo);
                    //write box num
                    writeDataIntoBUf(buf, 8 + strlen(addrInfo), BoxsDataLen);
                    //read boxs info into buf
                    read(readfd, buf + 8 + strlen(addrInfo) + 4, BoxsDataLen * 4);
                    //count buffer len
                    int32_t bufLen = 4 + 4 + strlen(addrInfo) + 4 + BoxsDataLen * 4;
                    write(client_sock, buf, bufLen);
                    bzero(buf, sizeof(buf));
                    break;
                }
                case CLIENT_LOGOFF_MESSAGE:
                {
                    close(client_sock);
                    return 0;
                }
                }
            }
        }
    }
}

//this thread is to process tcp link
void *client_line(void *arg)
{
    int client_sock = *((int *)arg);
    byte typeByte[4];
    while (true)
    {
        int dataLen = read(client_sock, typeByte, 4);
        int type = bytesto_int4(typeByte);
        switch (type)
        {
        case CLIENT_PORT_MESSAGE:
        {
            byte portBytes[4];
            int dataLen = read(client_sock, portBytes, 4);
            int port = bytesto_int4(portBytes);
            if (port <= 0 || port >= 65536)
            {
                cout << "port error!!!" << endl;
            }
            else
            {
                queue_port.push(port);
            }
            break;
        }
        }
    }
}

void *tranSenderUdpData(void *arg)
{
    int serv_sock = *((int *)arg);
    cout << "serv_sock: " << serv_sock << endl;
    //if client port no open waiting
    while (queue_port.size() == 0)
        ;
    //get a port from port queue
    int port = queue_port.front();
    queue_port.pop();
    //transmit int to char
    char port_c[255];
    sprintf(port_c, "%d", port);

    sockaddr_in receiver_addr;
    socklen_t receiver_addr_len;
    addr_init(&receiver_addr, AF_INET, CLIENT_IP, port_c);
    //init value
    byte packet[8 + 4 + 4 + 4 + MAX_SIZE]; //packet to store recieved data
    int64_t preTime = 0;
    map<int32_t, dataUnit> frameDataMap;
    int32_t count = 0; //the total packet of the frame
    int32_t dataTotalLen = 0;
    bool flag = false; //flag to get a frame
    while (true)
    {
        size_t packetLen = recvfrom(serv_sock, packet, sizeof(packet), 0, NULL, NULL);
        //get time
        byte timeBytes[8];
        arrayCopy(packet, 0, timeBytes, 0, 8);
        int64_t frameTime = bytesto_int8(timeBytes);
        sendto(serv_sock, packet, packetLen, 0, (sockaddr *)&receiver_addr, sizeof(receiver_addr));

        // if (frameTime - preTime >= 5000000 || flag == true) //if bigger than 3s get a frame
        // {
        //     if (frameTime < preTime)
        //     {
        //         continue;
        //     }
        //     else
        //     {
        //         if (frameTime > preTime)
        //         {
        //             //if new frame arrived
        //             preTime = frameTime;
        //             frameDataMap.clear();
        //             dataTotalLen = 0;
        //             //get count
        //             byte countBytes[4];
        //             arrayCopy(packet, 8, countBytes, 0, 4);
        //             count = bytesto_int4(countBytes);
        //             flag = true;
        //         }
        //         //get order
        //         byte orderBytes[4];
        //         arrayCopy(packet, 12, orderBytes, 0, 4);
        //         int32_t order = bytesto_int4(orderBytes);
        //         //get data len
        //         byte dataLenBytes[4];
        //         arrayCopy(packet, 16, dataLenBytes, 0, 4);
        //         int32_t datalen = bytesto_int4(dataLenBytes);
        //         dataTotalLen += datalen;
        //         //get data
        //         byte *dataBytes = new byte[datalen];
        //         bzero(dataBytes, datalen);
        //         arrayCopy(packet, 20, dataBytes, 0, datalen);
        //         //add in the map
        //         frameDataMap.insert(pair<int32_t, dataUnit>(order, dataUnit(datalen, dataBytes)));
        //         dataUnit dataBytes1 = frameDataMap[order];
        //         //if frame packets is all arrived and convert it to a picture
        //         if (frameDataMap.size() == count)
        //         {
        //             byte *frameData = new byte[dataTotalLen];
        //             getFrameData(frameDataMap, count, frameData);
        //             string fp_name = to_string(frameTime) + ".jpg";
        //             FILE *fp = fopen(fp_name.c_str(), "ab");
        //             fwrite(frameData, 1, dataTotalLen, fp);
        //             delete[] frameData;
        //             flag = false;
        //         }
        //     }
        // }
    }
    close(serv_sock);
    return 0;
}

// Mat frame;
// vector<byte> d(frameData, frameData + dataTotalLen);
// frame = imdecode(Mat(d), CV_LOAD_IMAGE_COLOR);
// imshow("picture", frame);
// delete[] frameData;
// char t = waitKey(30);
// if (t == 27)
// {
//     break;
// }