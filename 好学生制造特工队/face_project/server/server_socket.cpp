#include "server_socket.h"
#include <arpa/inet.h>
#include "iostream"
#include "string.h"

server_socket::server_socket(const char *serv_port)
{
    socket_handle = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_handle < 0)
    {
        std::cerr << "Socket failed. Socket was not initialized." << std::endl;
    }
    bzero(&sock_addr, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(atoi(serv_port));
}

bool server_socket::bindSocketAndListen(const int backlog) const
{
    int bind_status = bind(socket_handle, (sockaddr *)&sock_addr, sizeof(sock_addr));

    if (bind_status < 0)
    {
        std::cerr << "Binding failed. Could not bind the socket." << std::endl;
        return false;
    }

    int listen_status = listen(socket_handle, backlog);
    if (listen_status < 0)
    {
        std::cerr << "Listen failed. Could not listen the socket." << std::endl;
        return false;
    }

    return true;
}

int server_socket::getSocketHandle()
{
    return socket_handle;
}
