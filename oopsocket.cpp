#include "oopsocket.h"
#include <unistd.h>
#include<arpa/inet.h>
#include<fcntl.h>
OopSOCKET::OopSOCKET()
{
    int fd=socket(AF_INET,SOCK_STREAM,0);
    assert(fd>0);
    fcntl(fd,F_SETFL,O_NONBLOCK);
    sockid_=fd;
}
OopSOCKET::~OopSOCKET()
{
    close(sockid_);
}
bool OopSOCKET::bindIP(const std::string& port)
{
    assert(port.size()<=5);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_port=htonl(atoi(port.c_str()));
    bool result=bind(sockid_,(sockaddr*)&addr,sizeof(addr))==0;
    return result;
}
bool OopSOCKET::setsndbufsize(long bufsize)
{
    bool result=setsockopt(sockid_,SOL_SOCKET,SO_SNDBUF,(void*)bufsize,sizeof(bufsize));
    return result;
}
bool OopSOCKET::setrecbufsize(long bufsize)
{
    bool result=setsockopt(sockid_,SOL_SOCKET,SO_RCVBUF,(void*)bufsize,sizeof(bufsize));
    return result;
}
bool OopSOCKET::Connect(const std::string &ipaddr, const std::string &port)
{
    sockaddr_in addr;
    addr.sin_addr.s_addr=inet_addr(ipaddr.c_str());
    addr.sin_family=AF_INET;
    addr.sin_port=htonl(atoi(port.c_str()));
    bool result=connect(sockid_,(sockaddr*)&addr,sizeof(addr))==0;
    return result;
}
bool OopSOCKET::listenconnect()
{
    bool result=listen(sockid_,100)==0;
    return result;
}
bool OopSOCKET::shutdownsock(int flag)
{
    bool result=shutdown(sockid_,flag);
    return result;
}
void* OopSOCKET::recvtobuffer(void* buf)
{
    bytebuf temp;
    int n=recv(sockid_,(void*)temp.buf,temp.length,0);
    if(n>0)
        temp.length=n;
    return (void*)n;
}
void* OopSOCKET::sendfrombuffer(void* buf)
{
    bytebuf temp;
    int n=send(sockid_,(void*)temp.buf,temp.length,0);
    return (void*)n;
}
