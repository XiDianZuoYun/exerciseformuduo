#include "base/eventloop.h"
#include "base/buffer.h"
#include "base/tcpconnection.h"
auto echo=[&](Buffer* b,TcpConnection* t)
{
    char* temp=b->takedata(b->getSize());
    t->Send(temp,b->getSize());
    free(static_cast<void*>(temp));
};
int main()
{
    TcpConnection::CallBack callback=echo;
    EventLoop loop(1024);
    loop.Bind_Port(8888);
    loop.setDefaultCallback(callback);
    loop.loop();
    loop.stop();
    return 0;
}
