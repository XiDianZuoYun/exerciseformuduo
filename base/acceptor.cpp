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
    TcpConnection* new_connection=ALLOCATE(TcpConnection) TcpConnection(in_socket->Accept(),1024,TcpServer_);
    new_connection->setMessageCallback(default_CB);
    TcpServer_->UpdateConnection(TcpServer::conptr(new_connection));
}
