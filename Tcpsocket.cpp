#include <unistd.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include "Tcpsocket.h"
TcpSOCKET::TcpSOCKET()
{
    int fd=socket(AF_INET,SOCK_STREAM,0);
    assert(fd>0);
    fcntl(fd,F_SETFL,O_NONBLOCK);
    sockid_=fd;
}
TcpSOCKET::TcpSOCKET(TcpSOCKET &&o):sockid_(o.getfd())
{
    o.restore();
    o.~TcpSOCKET();
}
TcpSOCKET& TcpSOCKET::operator =(TcpSOCKET &&o)
{
    this->sockid_=o.getfd();
    o.restore();
    o.~TcpSOCKET();
    return *this;
}
TcpSOCKET::~TcpSOCKET()
{
    close(sockid_);
}
bool TcpSOCKET::bindport(const std::string& port)
{
    assert(port.size()<=5);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=htonl(INADDR_ANY);
    addr.sin_port=htonl(atoi(port.c_str()));
    bool result=bind(sockid_,(sockaddr*)&addr,sizeof(addr))==0;
    return result;
}
bool TcpSOCKET::setsndbufsize(long bufsize)
{
    bool result=setsockopt(sockid_,SOL_SOCKET,SO_SNDBUF,(void*)bufsize,sizeof(bufsize));
    return result;
}
bool TcpSOCKET::setrecbufsize(long bufsize)
{
    bool result=setsockopt(sockid_,SOL_SOCKET,SO_RCVBUF,(void*)bufsize,sizeof(bufsize));
    return result;
}
bool TcpSOCKET::Connect(const std::string &ipaddr, const std::string &port)
{
    sockaddr_in addr;
    addr.sin_addr.s_addr=inet_addr(ipaddr.c_str());
    addr.sin_family=AF_INET;
    addr.sin_port=htonl(atoi(port.c_str()));
    bool result=connect(sockid_,(sockaddr*)&addr,sizeof(addr))==0;
    return result;
}
bool TcpSOCKET::listenconnect()
{
    bool result=listen(sockid_,100)==0;
    return result;
}
bool TcpSOCKET::shutdownsock(int flag)
{
    bool result=shutdown(sockid_,flag);
    return result;
}
void* TcpSOCKET::recvtobuffer(void* buf,int length)
{
    int n=recv(sockid_,buf,length,0);
    return (void*)n;
}
void* TcpSOCKET::sendfrombuffer(void* buf,int length)
{
    int n=send(sockid_,(void*)buf,length,0);
    return (void*)n;
}
