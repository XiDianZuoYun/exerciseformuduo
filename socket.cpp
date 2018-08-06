#include "socket.h"
#include <QDebug>
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
void Socket::Bind(unsigned int port)
{
    struct sockaddr_in temp;
    bzero(&temp,sizeof(sockaddr_in));
    temp.sin_family=AF_INET;
    temp.sin_addr.s_addr=INADDR_ANY;
    temp.sin_port=htons(port);
    assert(bind(sock_fd,(sockaddr*)&temp,sizeof(sockaddr_in))==0);
}
void Socket::Listen(int backlogs)
{
    assert(listen(sock_fd,backlogs)>=0);
    qDebug()<<"listened";
}
void Socket::Block(bool block)
{
    int __block=block?~O_NONBLOCK:O_NONBLOCK;
    assert(fcntl(sock_fd, F_SETFL, fcntl(sock_fd, F_GETFL, 0) | __block)==0);
}
void Socket::Connect(const char *ip, int port)
{
    struct in_addr addr;
    assert(inet_pton(AF_INET,ip,(void*)&addr)==1);
    sockaddr_in temp;
    bzero(&temp,sizeof(sockaddr_in));
    temp.sin_family=AF_INET;
    temp.sin_port=htonl(port);
    temp.sin_addr.s_addr=addr.s_addr;
    assert(connect(sock_fd,(sockaddr*)&temp,sizeof(sockaddr_in))==0);
}
int Socket::Accept()
{
    struct sockaddr_in temp;
    bzero(&temp,sizeof(sockaddr_in));
    int addr_len=sizeof(sockaddr_in);
    int peer_socket=accept(sock_fd,(sockaddr*)&temp,(socklen_t*)&addr_len);
//    std::cout<<peer_socket;
    assert(peer_socket>=0||errno==EAGAIN);
    return peer_socket;
}
void Socket::SetReadBuf(int val)
{
    assert(setsockopt(sock_fd,SOL_SOCKET,SO_RCVBUF,&val,sizeof(int))==0);
}
void Socket::SetSendBuf(int val)
{
     assert(setsockopt(sock_fd,SOL_SOCKET,SO_SNDBUF,&val,sizeof(int))==0);
}
int Socket::Send(unsigned char *buf, int buf_len)
{
    assert(sock_type==SOCKET_TCP);
    assert(buf_len>0&&buf!=nullptr);
    int left=buf_len;
    while (left>0) {
       int len=send(sock_fd,(void*)buf,buf_len,0);
       assert(len>=0);
       if(len==0)
           break;
       left=left-len;
    }
    std::cout<<"We have sended "<<buf_len-left<<"bytes";
    return buf_len-left;
}
int Socket::Recv(unsigned char *buf, int &buf_len)
{
    assert(sock_type==SOCKET_TCP);
    assert(buf_len>0&&buf!=nullptr);
    buf_len=recv(sock_fd,(void*)buf,buf_len,0);
    return buf_len;
}
int Socket::Sendto(unsigned char *buf, int buf_len, const char *ip, int port)
{
    assert(sock_type==SOCKET_UDP);
    assert(buf!=nullptr&&buf_len>0);
    in_addr addr;
    assert(inet_pton(AF_INET,ip,(void*)&addr)==1);
    struct sockaddr_in temp;
    bzero(&temp,sizeof(sockaddr_in));
    temp.sin_port=htonl(port);
    temp.sin_addr.s_addr=addr.s_addr;
    temp.sin_family=AF_INET;
    int len=sendto(sock_fd,(void*)buf,buf_len,0,(sockaddr*)&temp,sizeof(sockaddr_in));
    assert(len>=0);
}
int Socket::Recvfrom(unsigned char *buf, int &buf_len, char *ip, int& port)
{
    assert(sock_type==SOCKET_UDP);
    sockaddr_in temp;
    bzero(&temp,sizeof(sockaddr_in));
    int addr_len=sizeof(sockaddr_in);
    int len=recvfrom(sock_fd,(void*)buf,buf_len,0,(sockaddr*)&temp,(socklen_t*)&addr_len);
    assert(len>=0);
    assert(inet_ntop(AF_INET,(void*)&(temp.sin_addr),ip,sizeof(in_addr))!=nullptr);
    port=ntohl(temp.sin_port);
    return len;
}
