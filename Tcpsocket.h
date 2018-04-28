#ifndef OOPSOCKET_H
#define OOPSOCKET_H
#include <sys/socket.h>
#include <assert.h>
#include<arpa/inet.h>
#include <string>
#include <buffer.hpp>
class TcpSOCKET
{
public:

    TcpSOCKET(TcpSOCKET&& o);
    TcpSOCKET& operator=(TcpSOCKET&& _o);
    TcpSOCKET();
    ~TcpSOCKET();
    bool bindport(const std::string& port);
    bool setsndbufsize(long bufsize);
    bool setrecbufsize(long bufsize);
    bool listenconnect();
    bool Connect(const std::string& ipaddr,const std::string& port);
    bool shutdownsock(int flag);
    void* recvtobuffer(void* buf,int length);
    void* sendfrombuffer(void* buf,int length);
    int getfd(){return sockid_;}
    void restore(){sockid_=-1;}
private:
    int sockid_;
};
#endif // OOPSOCKET_H
