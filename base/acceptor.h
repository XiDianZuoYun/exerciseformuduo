#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include <unordered_map>
#include <queue>
#include <iostream>
#include <fcntl.h>
#include <jemalloc/jemalloc.h>
#include <errno.h>
#include "channel.h"
#include "socket.h"
#include "tcpconnection.h"
class EventLoop;
class TcpServer;
class Acceptor
{
public:
    typedef Channel::functor functor;
    Acceptor(TcpServer* ts):in_socket(new Socket("TCP")),TcpServer_(ts)
    {
        init_Channel();
    }
    Acceptor(Socket* portsock,uint16_t port__):in_socket(portsock),port(port__)
    {
        init_Channel();
    }
    ~Acceptor() noexcept{
        delete in_socket;
        delete read_channel;
    }
    void setCallBack(TcpConnection::CallBack c)
    {
        default_CB=c;
    }
    void Bind(uint16_t Port);
    void Bind(std::string& ip,uint16_t Port);
    void Listen(int backlog=100){in_socket->Listen(backlog);}
    uint16_t GetPort(){return port;}
    Channel* getChannel(){return read_channel;}
    Acceptor(const Acceptor& other)=delete;
    Acceptor& operator=(const Acceptor& another) = delete;
private:
    TcpConnection::CallBack default_CB;
    int idlefd;
    void Accept_socket();
    void Accept_connection();
    void init_Channel();
    Socket *in_socket;
    Channel *read_channel;
    TcpServer* TcpServer_;
    uint16_t port;
    //std::queue<Socket*> freePool;
};
#endif // ACCEPTOR_H
