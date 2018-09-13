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
    void SetSendBuf(int val);
    void SetReadBuf(int val);
    void Block(bool block);
    void Bind(unsigned int port);
    void Listen(int backlogs=1024);
    void Connect(const char* ip,int port);
    int Accept();
    int Send(/*const Buffer& buf*/unsigned char* buf,int buf_len);
    int Recv(/*const Buffer& buf*/unsigned char* buf,int& buf_len);
    int Sendto(/*const Buffer& buf*/unsigned char* buf, int buf_len, const char* ip, int port);
    int Recvfrom(/*const Buffer& buf*/unsigned char* buf, int& buf_len, char *ip, int &port);
    int getfd(){return sock_fd;}
    ~Socket(){close(sock_fd);}
private:
    int sock_fd;
    sockaddr_in peer_addr;
    int sock_type;
};

#endif // SOCKET_H
