#include <iostream>
#include "tcpconnection.h"
#include "eventloop.h"
#include "tcpserver.h"
TcpConnection::TcpConnection(int sock_fd,sockaddr_in* peer_addr,size_t bufsize,TcpServer* T):sock(new Socket(sock_fd,peer_addr)),
    buf(new Buffer(bufsize)),connect_channel(new Channel(sock_fd,EPOLLET|EPOLLIN)),
    TcpServer_(T)
{

    InitChannel();
}
TcpConnection::TcpConnection(Socket* sock__,size_t bufsize,TcpServer* T):sock(sock__),buf(new Buffer(bufsize)),Wbuf(new Buffer(bufsize)),
    connect_channel(new Channel(sock__->getfd(),EPOLLET|EPOLLIN)),TcpServer_(T)
{
    InitChannel();
    std::cout<<"new connection!!!"<<std::endl;
}
TcpConnection::~TcpConnection() noexcept
{
    delete sock;
    delete buf;
    delete Wbuf;
    //If this object was registered in a loop-object,loop will completely remove this object
    // and its channel object.
    if(TcpServer_!=nullptr)
        TcpServer_->Remove_Connection(*this);
    else
        delete connect_channel;
}
void TcpConnection::GetoBuf()
{
    int32_t len=buf->readfd(sock->getfd());
    assert(len>=0);
    if(len==0)
    {
        TcpServer_->Regist_Connection(this);
        return;
    }
    if(MessageCB!=nullptr)
        MessageCB(buf,this);
}
void TcpConnection::WritetoSock()
{
    int32_t len=Wbuf->getSize();
    if(len==0)
        return ;
    int32_t temp=Wbuf->writefd(sock->getfd(),len);
    if(temp==len)
        connect_channel->disable_write();
    else
        connect_channel->enable_write();
}
inline void TcpConnection::InitChannel()
{
    functor func=std::bind(&TcpConnection::GetoBuf,this);
    connect_channel->setreadCallback(func);
    functor wfunc=std::bind(&TcpConnection::WritetoSock,this);
    connect_channel->setWriteCallback(wfunc);
    functor hfunc=std::bind(&TcpConnection::HupHandle,this);
    connect_channel->setErrorCallback(hfunc);
}
void TcpConnection::HupHandle()
{
    if(TcpServer_)
        TcpServer_->Regist_Connection(this);
}
int32_t TcpConnection::Send(Buffer &buf, int buf_len)
{
    int temp_len=buf.writefd(sock->getfd(),buf_len);
    if(temp_len==buf_len)
        return temp_len;
    std::pair<const char*,int32_t> p=buf.readdata();
    Wbuf->wirtein(p.first,p.second);
    connect_channel->enable_write();
}
