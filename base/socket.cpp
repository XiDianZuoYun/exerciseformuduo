#include "socket.h"
#include "acceptor.h"
#include <QDebug>
#define SOCKTYPE(str) !strcmp(str,"TCP")||!strcmp(str,"tcp")?SOCKET_TCP:SOCKET_UDP
Socket::Socket(const char *type)
{
    if(!strcmp(type,"TCP")||!strcmp(type,"tcp"))
    {
        sock_type=SOCKET_TCP;
        sock_fd=socket(AF_INET,SOCK_STREAM,0);
    }else if(!strcmp(type,"UDP")||!strcmp(type,"udp"))
    {
        sock_type=SOCKET_UDP;
        sock_fd=socket(AF_INET,SOCK_DGRAM,0);
    }
    assert(sock_fd>0);
    std::cout<<errno;
}
Socket::Socket(int32_t fd,sockaddr_in *addr,const char* type):sock_fd(fd),
    peer_addr(addr),sock_type(SOCKTYPE(type))
{

}
void Socket::Bind(uint16_t port)
{
    struct sockaddr_in temp;
    bzero(&temp,sizeof(sockaddr_in));
    temp.sin_family=AF_INET;
    temp.sin_addr.s_addr=htonl(INADDR_ANY);
    temp.sin_port=htons(port);
    assert(bind(sock_fd,(sockaddr*)&temp,sizeof(sockaddr_in))==0);
    this->port=port;
}
void Socket::Bind(std::string &ip, uint16_t port)
{
    struct sockaddr_in temp;
    bzero(&temp,sizeof(sockaddr_in));
    temp.sin_family=AF_INET;
    temp.sin_addr.s_addr=inet_addr(ip.c_str());
    temp.sin_port=htons(port);
    assert(bind(sock_fd,(sockaddr*)&temp,sizeof(sockaddr_in))==0);
    this->port=port;
    this->ip=ip;
}
void Socket::Listen(int backlogs)
{
    assert(listen(sock_fd,backlogs)>=0);
    //qDebug()<<"listened";
}
void Socket::Block(bool block)
{
    int32_t __block=block?~O_NONBLOCK:O_NONBLOCK;
    assert(fcntl(sock_fd, F_SETFL, fcntl(sock_fd, F_GETFL, 0) | __block)==0);
}
void Socket::Connect(const char *ip, uint16_t port)
{
    struct in_addr addr;
    assert(inet_pton(AF_INET,ip,(void*)&addr)==1);
    sockaddr_in temp;
    bzero(&temp,sizeof(sockaddr_in));
    temp.sin_family=AF_INET;
    temp.sin_port=htons(port);
    temp.sin_addr.s_addr=addr.s_addr;
    assert(connect(sock_fd,(sockaddr*)&temp,sizeof(sockaddr_in))==0);
}
int32_t Socket::Accept(sockaddr_in* peer)
{
    assert(peer!=nullptr);
    int32_t addr_len=sizeof(sockaddr_in);
    int32_t peer_socket=accept(sock_fd,(sockaddr*)peer,(socklen_t*)&addr_len);
//    std::cout<<peer_socket;
    assert(peer_socket>=0||errno==EAGAIN);
    return peer_socket;
}
Socket* Socket::Accept()
{
    struct sockaddr_in* temp=new sockaddr_in;
    bzero(temp,sizeof(sockaddr_in));
    bzero(temp,sizeof(sockaddr_in));
    int peer_socket=Accept(temp);
    assert(peer_socket>=0||errno==EAGAIN||errno==EMFILE);
    return peer_socket>0?new Socket(peer_socket,temp,"TCP"):nullptr;
}
void Socket::SetReadBuf(int32_t val)
{
    assert(setsockopt(sock_fd,SOL_SOCKET,SO_RCVBUF,&val,sizeof(int32_t))==0);
}
void Socket::SetSendBuf(int32_t val)
{
     assert(setsockopt(sock_fd,SOL_SOCKET,SO_SNDBUF,&val,sizeof(int32_t))==0);
}
int32_t Socket::Send( char *buf, int32_t buf_len)
{
    assert(sock_type==SOCKET_TCP);
    assert(buf_len>0&&buf!=nullptr);
    int32_t left=buf_len;
    while (left>0) {
       int32_t len=send(sock_fd,(void*)buf,buf_len,0);
       assert(len>=0);
       if(len==0)
           break;
       left=left-len;
    }
    std::cout<<"We have sended "<<buf_len-left<<"bytes";
    return buf_len-left;
}
int32_t Socket::Recv( char *buf, int32_t &buf_len)
{
    assert(sock_type==SOCKET_TCP);
    assert(buf_len>0&&buf!=nullptr);
    buf_len=recv(sock_fd,(void*)buf,buf_len,0);
    return buf_len;
}
int32_t Socket::Sendto( char *buf, size_t buf_len, const char *ip, uint16_t port)
{
    assert(sock_type==SOCKET_UDP);
    assert(buf!=nullptr&&buf_len>0);
    in_addr addr;
    assert(inet_pton(AF_INET,ip,(void*)&addr)==1);
    struct sockaddr_in temp;
    bzero(&temp,sizeof(sockaddr_in));
    temp.sin_port=htons(port);
    temp.sin_addr.s_addr=addr.s_addr;
    temp.sin_family=AF_INET;
    int32_t len=sendto(sock_fd,(void*)buf,buf_len,0,(sockaddr*)&temp,sizeof(sockaddr_in));
    assert(len>=0);
}
int32_t Socket::Recvfrom( char *buf, size_t &buf_len, char *ip, uint16_t &port)
{
    assert(sock_type==SOCKET_UDP);
    sockaddr_in temp;
    bzero(&temp,sizeof(sockaddr_in));
    int32_t addr_len=sizeof(sockaddr_in);
    int32_t len=recvfrom(sock_fd,(void*)buf,buf_len,0,(sockaddr*)&temp,(socklen_t*)&addr_len);
    assert(len>=0);
    assert(inet_ntop(AF_INET,(void*)&(temp.sin_addr),ip,sizeof(in_addr))!=nullptr);
    port=ntohs(temp.sin_port);
    return len;
}
