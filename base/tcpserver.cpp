#include "tcpserver.h"

TcpServer::TcpServer(unsigned int maxconnections, int backlog, uint16_t port):loop_(new EventLoop(maxconnections,this)),
    acceptor(new Acceptor(this)),backlog(backlog)
{
    acceptor->Bind(port);
    loop_->updateChannel(acceptor->getChannel());
}
TcpServer::~TcpServer() noexcept
{
    for(auto it:Con_map)
        it.second.reset();
    delete acceptor;
    delete loop_;
}
void TcpServer::Run()
{
    acceptor->Listen(backlog);
    loop_->loop();
}
void TcpServer::Stop()
{
    loop_->stop();
}
