#ifndef SOCKET_H
#define SOCKET_H
#include <string.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sendfile.h>
#include<arpa/inet.h>
#include "buffer.h"
#define SOCKET_UDP 0
#define SOCKET_TCP 1
class Acceptor;
class Socket
{
public:
    typedef sockaddr_in inetaddr;
    Socket(const char* type="TCP");
    ~Socket(){
        Close();
        delete(peer_addr);
    }
    Socket(int32_t fd,sockaddr_in *addr=nullptr,const char* type="TCP");
    Socket(const Socket& sock)=delete ;
    Socket& operator=(const Socket& sock)=delete;
    void SetSendBuf(int32_t val);
    void SetReadBuf(int32_t val);
    void Block(bool block);
    void Bind(uint16_t port);
    void Bind(std::string& ip,uint16_t port);
    void Listen(int backlogs=1024);
    void Connect(const char* ip, uint16_t port);
    int32_t Accept(struct sockaddr_in* peer);
    Socket* Accept();
    int32_t Send( char* buf,int32_t buf_len);
    int32_t Send(Buffer& buf,int32_t buf_len)
    {
        return Send(buf.getdata(buf_len),buf_len);
    }
    int32_t Recv( char* buf,int32_t& buf_len);
    int32_t Recv(Buffer& buf)
    {
        return buf.readfd(sock_fd);
    }
    int32_t Sendto( char* buf, size_t buf_len, const char* ip, uint16_t port);
    int32_t Sendto(Buffer& buf,size_t buf_len,const char* ip,uint16_t port)
    {
        return Sendto(buf.getdata(buf_len),buf_len,ip,port);
    }
    int32_t Recvfrom( char* buf, size_t &buf_len, char *ip, uint16_t &port);
    int32_t getfd(){return sock_fd;}
    void Close(){
        assert(close(sock_fd)==0);
    }
    ssize_t SendFile(int outfd,int infd,off_t* offset,size_t count)
    {
        return sendfile(outfd,infd,offset,count);
    }
    void Reset(int Sockfd,inetaddr* addr)
    {
        this->sock_fd=Sockfd;
        memcpy((void*)peer_addr,(void*)addr,sizeof(inetaddr));
    }
private:
    int32_t sock_fd;
    inetaddr *peer_addr=nullptr;
    int32_t sock_type;
    std::string ip;
    uint16_t port;
};

#endif // SOCKET_H
