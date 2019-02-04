#include "sender_socket.h"
#include "iostream"
#include "string.h"

//init socket
sender_socket::sender_socket(
    const std::string &serv_ip, const char* serv_port)
{
    socket_handle = socket(AF_INET, SOCK_STREAM, 0);
    //argv[1] --->  ip ; argv[2] ----> port
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(serv_port));
    serv_addr.sin_addr.s_addr = inet_addr(serv_ip.c_str());
}

bool sender_socket::connectToServer()
{
    int ret = connect(socket_handle, (sockaddr *)&serv_addr, sizeof(serv_addr));
    if(ret == -1)
        return false;
    else
        return true;
}

int sender_socket::getSocketHandle()
{
    return socket_handle;
}
