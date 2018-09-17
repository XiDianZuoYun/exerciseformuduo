#ifndef SOCKET_H
#define SOCKET_H
#include <string.h>
#include <fcntl.h>
#include <iostream>
#include <sys/socket.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include<arpa/inet.h>
#define SOCKET_UDP 0
#define SOCKET_TCP 1
class Socket
{
public:
    Socket(const char* type="TCP");
    ~Socket(){
        close(sock_fd);
        delete(peer_addr);
    }
    Socket(int32_t fd,sockaddr_in *addr=nullptr,const char* type="TCP");
    Socket(const Socket& sock)=delete ;
    Socket& operator=(const Socket& sock)=delete;
    void SetSendBuf(int32_t val);
    void SetReadBuf(int32_t val);
    void Block(bool block);
    void Bind(uint16_t port);
    void Listen(int backlogs=1024);
    void Connect(const char* ip, uint16_t port);
    int32_t Accept(struct sockaddr_in* peer);
    Socket* Accept();
    int32_t Send(/*const Buffer& buf*/unsigned char* buf,int32_t buf_len);
    int32_t Recv(/*const Buffer& buf*/unsigned char* buf,int32_t& buf_len);
    int32_t Sendto(/*const Buffer& buf*/unsigned char* buf, size_t buf_len, const char* ip, uint16_t port);
    int32_t Recvfrom(/*const Buffer& buf*/unsigned char* buf, size_t &buf_len, char *ip, int32_t &port);
    int32_t getfd(){return sock_fd;}
private:
    int32_t sock_fd;
    sockaddr_in *peer_addr=nullptr;
    int32_t sock_type;
    uint16_t port;
};

#endif // SOCKET_H
