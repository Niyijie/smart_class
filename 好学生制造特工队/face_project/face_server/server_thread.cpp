#include "../common/sockets.h"
#include "../common/util.h"
#include "map"
#include "queue"
#include "pthread.h"

using namespace std;

#define RECIEVER_IP "192.168.1.119"
#define LISTEN_PORT "8888"

queue<dataUnit> dataList;
//connect setting
sockaddr_in serv_addr,send_addr, sender_addr, listen_addr, reciever_addr;
int serv_sock = Socket(PF_INET, SOCK_DGRAM, 0);
int listen_sock = Socket(PF_INET, SOCK_DGRAM, 0);
socklen_t sender_addr_len, reciever_addr_len,send_addr_len;

void *recieveData(void *arg)
{   
    int len = 8 + 4 + 4 + 4 + MAX_SIZE;
    addr_init(&listen_addr, AF_INET, INADDR_ANY, LISTEN_PORT); //init listen socket to check login 
    Bind(listen_sock, &listen_addr, sizeof(listen_addr));
    while (true)
    {   
        byte *packet = new byte[len]; //packet to store recieved data
        //get time
        byte frameTimeBytes[8];
        arrayCopy(packet,0,frameTimeBytes,0,8);
        int64_t time = bytesto_int8(frameTimeBytes);
        //get total
        byte totalBytes[4];
        arrayCopy(packet,8,totalBytes,0,4);
        int32_t total = bytesto_int4(totalBytes);
        //get order
        byte orderBytes[4];
        arrayCopy(packet,12,orderBytes,0,4);
        int32_t order = bytesto_int4(orderBytes);
        
        cout<<time<<" "<<total<<" "<<order<<endl;
        sender_addr_len = sizeof(sockaddr);
        size_t packetLen = recvfrom(listen_sock, packet, len, 0, NULL, NULL);
        cout<<"datalen "<< packetLen<<endl;
        dataUnit unit(packetLen, packet);
        dataList.push(unit);
    }
}
//172.27.85.72
void *sendData(void *arg)
{   
    byte login_info[4] = "2";
//    send(serv_sock, login_info, 4, 0);
 //   send(serv_sock, login_info, 4, 0);
    while (true)
    {   
        cout<<dataList.size()<<endl;
        if (dataList.size() > 0)
        {   
            byte *packet = dataList.front().dataBytes;
            byte info[4] = "1";
            size_t packetLen = dataList.front().dataLen;
            dataList.pop(); 
            send(serv_sock, packet,packetLen, 0);
            cout<<"sent len " << packetLen <<endl;
            delete packet;
        }
        else
        {   
            sleep(1);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    addr_init(&serv_addr, AF_INET, INADDR_ANY, argv[1]);       //init server socket
    //addr_init(&listen_addr, AF_INET, INADDR_ANY, LISTEN_PORT); //init listen socket to check login
    Bind(serv_sock, &serv_addr, sizeof(serv_addr));

    //Bind(listen_sock, &listen_addr, sizeof(listen_addr));
    while (true)
    {
        reciever_addr_len = sizeof(sockaddr);
        byte login_info[4];
        cout<<"listen"<<endl;
        size_t packetLen = recvfrom(serv_sock, login_info, sizeof(login_info), 0, (sockaddr *)&reciever_addr, &reciever_addr_len);
        // getpeername(listen_sock,  (sockaddr*)&reciever_addr,&reciever_addr_len);
        int id = connect(serv_sock,(sockaddr*)&reciever_addr,reciever_addr_len);
        printf("%d frome %s ,port %d \n", reciever_addr_len, inet_ntoa(reciever_addr.sin_addr), ntohs(reciever_addr.sin_port));
        cout << packetLen << endl;
        if (packetLen > 0)
        {
            int32_t type = bytesto_int4(login_info);
            cout << type << endl;
            if (type == 10000)
            {
          //       sendto(serv_sock, login_info, packetLen, 0, (sockaddr *)&reciever_addr, reciever_addr_len);
                send(serv_sock, login_info, 4, 0);
                cout << "connet successfully" << endl;
        //         sendto(serv_sock, login_info, packetLen, 0, (sockaddr *)&reciever_addr, reciever_addr_len);
                break;
            }
        }
    }
    pthread_t  t_id1;
    pthread_t  t_id2;
    if (pthread_create(&t_id1, NULL, recieveData, NULL) != 0)
    {
        cout << "thread1 error" << endl;
        return -1;
    }

    if (pthread_create(&t_id2, NULL, sendData, NULL) != 0)
    {
        cout << "thread2 error" << endl;
        return -1;
    }
    pthread_join(t_id1,NULL);
    cout<<"id1"<<endl;
    pthread_join(t_id2,NULL);
    cout<<"id2"<<endl;
    close(serv_sock);
    close(listen_sock);
    return 0;
}

