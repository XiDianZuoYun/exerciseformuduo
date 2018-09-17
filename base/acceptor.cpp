#include "acceptor.h"
#include "eventloop.h"
#include <iostream>
#ifndef DEBUG
#define DEBUG
#endif
void Acceptor::Bind(uint16_t Port)
{
    this->port=Port;
    in_socket->Bind(Port);
}
void Acceptor::Accept_socket()
{
    Socket* sock=in_socket->Accept();
    reg_sock[sock->getfd()]=sock;
    Channel* chan=new Channel(sock->getfd(),EPOLLIN|EPOLLERR,loop,CHANNEL_SOCK);
    loop->updateChannel(chan);
    std::cout<<"new connection!";
}
void Acceptor::Accept_connection()
{
    TcpConnection* new_connection=new TcpConnection(in_socket->Accept(),1024,loop);
    loop->UpdateNewConnection(new_connection);
    reg_sock[new_connection->getSock()->getfd()]=new_connection->getSock();
}
void Acceptor::ClearAllSock()
{
    for(auto &s:reg_sock)
        (s.second)->~Socket();
}
