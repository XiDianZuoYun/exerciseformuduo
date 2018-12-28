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
    typedef std::function<void (Buffer*, TcpConnection*)> CallBack;
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
    int32_t Send(char* buf,int length)
    {
        return sock->Send(buf,length);
    }
    int32_t Send(Buffer &buf,int buf_len)
    {
        return sock->Send(buf,buf_len);
    }
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
private:
    void GetoBuf();
    void defa_callback(char*)
    {
        return;
    }
    inline void InitChannel();
    Socket* sock;
    Buffer* buf;
    CallBack MessageCB;
    Channel* connect_channel;
    EventLoop* loop;
};

#endif // TCPCONNECTION_H
