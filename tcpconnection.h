#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H
#include <string.h>
#include <functional>
#include "socket.h"
#include "buffer.h"
#include "channel.h"
class EventLoop;
class TcpConnection
{
public:
    TcpConnection(int sock_fd,sockaddr_in* peer_addr,size_t bufsize,EventLoop* loop__);
    TcpConnection(Socket* sock,size_t bufsize,EventLoop* lo);
    ~TcpConnection();
    typedef std::function<void ()> functor;
    typedef std::function<void (char*)> CallBack;
    void setMessageCallback(CallBack& func)
    {
        MessageCB=func;
    }
    int32_t GetMessage(char* buf_,int length)
    {
        return buf->getdata(buf_,length);
    }
    int32_t TakeMessage(char* buf_,int length)
    {
        return buf->takedata(buf_,length);
    }
    Socket* getSock() const
    {
        return sock;
    }
private:
    void GetoBuf();
    inline void InitChannel();
    Socket* sock;
    Buffer* buf;
    CallBack MessageCB;
    Channel* connect_channel;
    EventLoop* loop;
};

#endif // TCPCONNECTION_H
