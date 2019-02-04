#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <arpa/inet.h>
#include "string"

class server_socket
{
  public:
    explicit server_socket(const char *serv_port);
    bool bindSocketAndListen(const int backlog) const;
    int getSocketHandle();

  private:
    //the socket identifier (fd)
    int socket_handle;
    sockaddr_in sock_addr;
};

#endif