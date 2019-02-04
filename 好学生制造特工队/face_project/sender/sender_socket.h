#ifndef SENDER_SOCKET_H
#define SENDER_SOCKET_H

#include <arpa/inet.h>
#include "string"

class sender_socket
{
  public:
    sender_socket(const std::string &serv_ip,  const char* serv_port);
    bool connectToServer();
    int getSocketHandle();

  private:
    //the socket identifier (fd)
    int socket_handle;
    //server's address and port.
    sockaddr_in serv_addr;
};


#endif