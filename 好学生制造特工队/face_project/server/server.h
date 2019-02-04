#ifndef SERVER_H
#define SERVER_H

#include "map"
#include "string"
#include "../util/util.h"
#include "server_socket.h"

struct piInfo
{
    piInfo(){}
    piInfo(std::string addr,int _fd)
    {
        RTSPAdrr = addr;
        fd = _fd;
    }
    std::string RTSPAdrr;
    int fd;
};

class ProcessServer
{
  public:
    explicit ProcessServer(const char *serv_port);

    server_socket& getServerSocket() ;
    std::map<std::string, piInfo>& getPiSocketMap() ;
    int getClientSocketHandle() const;

    void mgsParseFactory(MESSAGETYPE type,int fd);

  private:
    server_socket socket;
    std::map<std::string, piInfo> piSocketMap;
    int clientSocketHandle;
};

#endif