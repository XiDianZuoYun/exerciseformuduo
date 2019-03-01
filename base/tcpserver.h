#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <unordered_map>
#include <memory>
#include <queue>
#include "tcpconnection.h"
#include "eventloop.h"
#include "acceptor.h"
class TcpServer
{
public:
    typedef std::shared_ptr<TcpConnection> conptr;
    TcpServer(unsigned int maxconnections=1024,int backlog=1000,uint16_t port=8888);
    TcpServer(const TcpServer& other)=delete;
    ~TcpServer() noexcept;
    void UpdateConnection(conptr tc){
        Con_map[tc->getSock()->getfd()]=tc;
        loop_->updateChannel(tc->connect_channel);
    }
    void Remove_Connection(TcpConnection& t)
    {
        int fd=t.getSock()->getfd();
        Con_map.erase(fd);
        loop_->RemoveChannel(fd);
    }
    void Regist_Connection(TcpConnection* t)
    {
        Unaviliable_connection.push(t);
    }
    void Clear_connection()
    {
        while(!Unaviliable_connection.empty())
        {
            TcpConnection* t=Unaviliable_connection.front();
            Unaviliable_connection.pop();
            Remove_Connection(*t);
        }
    }
    void SetDefaultCallback(TcpConnection::CallBack& CB)
    {
        acceptor->setCallBack(CB);
    }
    void Run();
    void Stop();
private:
    Acceptor* acceptor;
    std::unordered_map<int,conptr> Con_map;
    std::queue<TcpConnection*> Unaviliable_connection;
    EventLoop* loop_;
    int backlog;
};

#endif // TCPSERVER_H
