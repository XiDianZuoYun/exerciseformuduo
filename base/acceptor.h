#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include <unordered_map>
#include <queue>
#include <iostream>
#include <jemalloc/jemalloc.h>
#include "channel.h"
#include "socket.h"
#include "tcpconnection.h"
class EventLoop;
class Acceptor
{
public:
    typedef Channel::functor functor;
    Acceptor(EventLoop* __loop):in_socket(new Socket("TCP")),loop(__loop)
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
    void Bind(uint16_t Port);
    void Listen(int backlog=100){in_socket->Listen(backlog);}
    uint16_t GetPort(){return port;}
    Channel* getChannel(){return read_channel;}
    Acceptor(const Acceptor& other)=delete;
    Acceptor& operator=(const Acceptor& another) = delete;
    void ClearAllSock();
private:
    void Accept_socket();
    void Accept_connection();
    void init_Channel()
    {
        read_channel=new Channel(in_socket->getfd(),EPOLLIN|EPOLLERR);
        std::cout<<in_socket->getfd();
        functor func=std::bind(&Acceptor::Accept_connection,this);
        read_channel->setreadCallback(func);
    }
    Socket *in_socket;
    Channel *read_channel;
    uint16_t port;
    std::unordered_map<int32_t,Socket*> reg_sock;
    //std::queue<Socket*> freePool;
    EventLoop* loop;
};
#endif // ACCEPTOR_H
