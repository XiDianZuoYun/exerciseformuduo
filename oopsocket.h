#ifndef OOPSOCKET_H
#define OOPSOCKET_H
#include <sys/socket.h>
#include <assert.h>
#include<arpa/inet.h>
#include <string>
class OopSOCKET
{
public:
    typedef struct bytebuf
    {
    public:
        char buf[];
        int length;
        bytebuf() {}
    } bytebuf;
    OopSOCKET(const OopSOCKET& o)=delete;
    OopSOCKET operator=(OopSOCKET& _o)=delete;
    OopSOCKET(int skfd_):sockid_(skfd_){}
    OopSOCKET();
    ~OopSOCKET();
    bool bindIP(const std::string& port);
    bool setsndbufsize(long bufsize);
    bool setrecbufsize(long bufsize);
    bool listenconnect();
    bool Connect(const std::string& ipaddr,const std::string& port);
    bool shutdownsock(int flag);
    void* recvtobuffer(void* buf);
    void* sendfrombuffer(void* buf);
    int getfd(){return sockid_;}
private:
    int sockid_;
};
#endif // OOPSOCKET_H
