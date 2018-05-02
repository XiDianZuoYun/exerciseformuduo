#include "acceptor.hpp"
void Acceptor::init()
{
    assert(_socket->listenconnect());
    _channel=new Channel(_socket->getfd(),EPOLLET|EPOLLIN,128,loop);
    _channel->setReadCallback(std::bind(&Acceptor::acceptiniloop,this));
    loop->updateChannel(_channel);
}
void Acceptor::acceptiniloop()
{
    sockaddr addr;
    int len=sizeof(addr);
    int fd=accept(_socket->getfd(),&addr,(socklen_t*)&len);
    Channel* ch=new Channel(fd,EPOLLET|EPOLLIN,1024,loop);
    loop->updateChannel(ch);
}
