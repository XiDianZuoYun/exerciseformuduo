#include <iostream>
#include "tcpconnection.h"
#include "eventloop.h"
TcpConnection::TcpConnection(int sock_fd,sockaddr_in* peer_addr,size_t bufsize,EventLoop* loop_):sock(new Socket(sock_fd,peer_addr)),
    buf(new Buffer(bufsize)),connect_channel(new Channel(sock_fd,EPOLLET|EPOLLIN)),
    loop(loop_)
{

    InitChannel();
}
TcpConnection::TcpConnection(Socket* sock__,size_t bufsize,EventLoop* lo):sock(sock__),buf(new Buffer(bufsize)),
    connect_channel(new Channel(sock__->getfd(),EPOLLET|EPOLLIN)),loop(lo)
{
    InitChannel();
    std::cout<<"new connection!!!"<<std::endl;
}
TcpConnection::~TcpConnection() noexcept
{
    sock->~Socket();
    buf->~Buffer();
    //If this object was registered in a loop-object,loop will remove completely remove this object
    // and its channel object.
    if(loop!=nullptr)
        loop->Remove_Connection(this);
    else
        connect_channel->~Channel();
}
void TcpConnection::GetoBuf()
{
    int len=buf->readfd(sock->getfd());
    assert(len>=0);
}
inline void TcpConnection::InitChannel()
{
    functor func=std::bind(&TcpConnection::GetoBuf,this);
    connect_channel->setreadCallback(func);
    loop->updateChannel(connect_channel);
}
