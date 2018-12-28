#ifndef SOCKETS_H
#define SOCKETS_H

#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include "arpa/inet.h"
#include "stdlib.h"
#include "unistd.h"
#include <stdlib.h>
#include "cstdio"
#include "string"
#include "cstring"

void error_handing(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int Socket(int __domain, int __type, int __protocol)
{
    int fd = socket(__domain,__type,__protocol);
    if(fd == -1)
    {
        error_handing("socket() error");
    }
    return fd;
}

int Bind(int __fd, sockaddr_in *__addr, socklen_t __len)
{
    int ret = bind(__fd,(sockaddr*)__addr,__len);
    if(ret == -1)
    {
        error_handing("bind() error!");
    }
    return ret;
}

int addr_init(sockaddr_in *addr,int __domain,char *ip,char *port)
{
    if(ip == NULL)
    {
        ip = "0";
    }
    
    bzero(addr,sizeof(addr));
    addr->sin_family = __domain;
    addr->sin_addr.s_addr = inet_addr(ip);
    addr->sin_port = htons(atoi(port));
}


#endif