#include "base/tcpserver.h".h"
#include "base/buffer.h"
#include "base/tcpconnection.h"
auto echo=[&](Buffer* b,TcpConnection* t)
{
    t->Send(*b,b->getSize());
};
int main()
{
    TcpConnection::CallBack callback=echo;
    TcpServer TS(1024,1000,8888);
    TS.SetDefaultCallback(callback);
    TS.Run();
    TS.Stop();
    return 0;
}
