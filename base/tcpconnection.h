#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H
#include <string.h>
#include <functional>
#include "socket.h"
#include "buffer.h"
#include "channel.h"
class TcpServer;
class EventLoop;
class TcpConnection
{
public:
    TcpConnection(int sock_fd, sockaddr_in* peer_addr, size_t bufsize, TcpServer *T);
    TcpConnection(Socket* sock,size_t bufsize,TcpServer* T);
    ~TcpConnection();
    typedef std::function<void ()> functor;
    typedef std::function<void (Buffer*, TcpConnection*)> CallBack;
    void setMessageCallback(CallBack& func)
    {
        MessageCB=func;
    }
    void setHupCallback(functor &fc)
    {
        connect_channel->setErrorCallback(fc);
    }
    int32_t GetMessage(char* buf_,int length)
    {
        return buf->getdata(buf_,length);
    }
    int32_t TakeMessage(char* buf_,int length)
    {
        return buf->takedata(buf_,length);
    }
    int32_t Send(char* buf,int length);
    int32_t Send(Buffer &buf,int buf_len);
    Socket* getSock() const
    {
        return sock;
    }
    void Disconnect(){
        this->sock->Close();
    }
    ssize_t SendFile(int outfd,int infd,off_t* offset,size_t count)
    {
        return sock->SendFile(outfd,infd,offset,count);
    }
    friend class TcpServer;
private:
    void GetoBuf();
    void WritetoSock();
    inline void InitChannel();
    void HupHandle();
    Socket* sock;
    Buffer* buf;
    Buffer* Wbuf;
    CallBack MessageCB;
    Channel* connect_channel;
    TcpServer* TcpServer_;
};

#endif // TCPCONNECTION_H
