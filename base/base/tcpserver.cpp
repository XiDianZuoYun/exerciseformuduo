#include "tcpserver.h"

TcpServer::TcpServer(unsigned int maxconnections, int backlog, uint16_t port):loop_(new EventLoop(maxconnections,this)),
    acceptor(new Acceptor(this)),backlog(backlog)
{
    acceptor->Bind(port);
    loop_->updateChannel(acceptor->getChannel());
    wheel.resize(8);
    Channel::functor func=std::bind(&TcpServer::Clear_expire_connection,this);
    loop_->runEvery(func,1);
}
TcpServer::~TcpServer() noexcept
{
    for(auto it:Con_map)
        it.second.reset();
    delete acceptor;
    delete loop_;
    while(!Unaviliable_connection.empty())
    {
        auto ptr=Unaviliable_connection.front();
        Unaviliable_connection.pop();
        delete ptr;
    }
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
void TcpServer::Clear_expire_connection()
{
    step=(step+1)%8;
    for(auto ptr:wheel[step])
    {
        int fd=ptr->getSock()->getfd();
        conptr p=Con_map[fd];
        this->Remove_Connection(*p);
    }
    wheel[step].clear();
}
void TcpServer::Clear_connection()
{
    while(!Unaviliable_connection.empty())
    {
        TcpConnection* t=Unaviliable_connection.front();
        Unaviliable_connection.pop();
        Remove_Connection(*t);
        delete t;
    }
}
bool TcpServer::Ruse_conn(int sockfd, Socket::inetaddr *addr)
{
    if(Unaviliable_connection.empty())
        return  false;
    TcpConnection* ptr=Unaviliable_connection.front();
    Unaviliable_connection.pop();
    ptr->Reuse_Connection(sockfd,addr);
    this->UpdateConnection(conptr(ptr));
    return true;
}
