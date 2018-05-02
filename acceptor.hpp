#ifndef ACCEPTOR_HPP
#define ACCEPTOR_HPP
#include <functional>
#include "Tcpsocket.h"
#include "POLLER.hpp"
#include "eventloop.hpp"
#include "callbackexample.hpp"
class Acceptor
{
public:
    Acceptor(EventLoop* _loop):_socket(new TcpSOCKET()),loop(_loop){init();}
    void init();
    void acceptiniloop();
private:
    EventLoop* loop;
    TcpSOCKET* _socket;
    Channel* _channel;
};
#endif // ACCEPTOR_HPP
