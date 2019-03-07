#include "acceptor.h"
#include "eventloop.h"
#include "tcpserver.h"
#include <iostream>
#ifndef DEBUG
#define DEBUG
#endif
#ifndef ALLOCATE(TYPE)
#define ALLOCATE(TYPE) new(malloc(sizeof(TYPE)))
#endif
void Acceptor::Bind(uint16_t Port)
{
    this->port=Port;
    in_socket->Bind(Port);
    idlefd=::open("/dev/null",O_RDONLY|O_CLOEXEC);
}
void Acceptor::Bind(std::string &ip, uint16_t Port)
{
    this->port=Port;
    in_socket->Bind(ip,Port);
}
//void Acceptor::Accept_socket()
//{
//    Socket* sock=in_socket->Accept();
//    Channel* chan=ALLOCATE(Channel) Channel(sock->getfd(),EPOLLIN|EPOLLERR,loop,CHANNEL_SOCK);
//    std::cout<<"new connection!";
//}
void Acceptor::Accept_connection()
{
    Socket* sock=in_socket->Accept();
    if(!sock&&errno==EMFILE)
    {
        close(idlefd);
        int tempfd=::accept(in_socket->getfd(),nullptr,nullptr);
        close(tempfd);
        idlefd=::open("/dev/null",O_RDONLY|O_CLOEXEC);
    }
    TcpConnection* new_connection=ALLOCATE(TcpConnection) TcpConnection(in_socket->Accept(),1024,TcpServer_);
    new_connection->setMessageCallback(default_CB);
    TcpServer_->UpdateConnection(TcpServer::conptr(new_connection));
}
void Acceptor::init_Channel()
{
    read_channel=new Channel(in_socket->getfd(),EPOLLIN|EPOLLERR);
    std::cout<<in_socket->getfd();
    functor func=std::bind(&Acceptor::Accept_connection,this);
    read_channel->setreadCallback(func);
}
