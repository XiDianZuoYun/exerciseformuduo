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
        int fd=tc->getSock()->getfd();
        Con_map[fd]=tc;
        wheel[step].push_back(tc);
        loop_->updateChannel(tc->connect_channel);
    }
    void Remove_Connection(TcpConnection& t)
    {
        int fd=t.getSock()->getfd();
        if(Con_map.find(fd)==Con_map.end())
            return;
        Con_map.erase(fd);
        loop_->RemoveChannel(fd);
    }
    void Regist_Connection(TcpConnection* t)
    {
        Unaviliable_connection.push(t);
    }
    void Clear_connection();
    void SetDefaultCallback(TcpConnection::CallBack& CB)
    {
        acceptor->setCallBack(CB);
    }
    int GetConnectionNums()
    {
        return Con_map.size();
    }
    bool Ruse_conn(int sockfd,Socket::inetaddr* addr);
    void Run();
    void Stop();
private:
    void Clear_expire_connection();
    Acceptor* acceptor;
    std::unordered_map<int,conptr> Con_map;
    //Pointer in this queue can be reused by acceptor.
    std::queue<TcpConnection*> Unaviliable_connection;
    EventLoop* loop_;
    using connectionslot=std::vector<conptr>;
    std::vector<connectionslot> wheel;
    int step=0;
    int backlog;
};

#endif // TCPSERVER_H
