#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include <map>
#include <vector>
#include <unordered_map>
#include <memory>
#include <assert.h>
#include "timer.h"
#include "poller.h"
#include "channel.h"
#include "acceptor.h"
#include "tcpconnection.h"
#ifdef DEBUG
#include <iostream>
#endif
class EventLoop
{
public:
    typedef std::shared_ptr<Channel> ChanPTR;
    EventLoop(int maxevents);
    ~EventLoop();
    EventLoop(const EventLoop& other)=delete;
    EventLoop& operator =(const EventLoop& other)=delete;
    void Bind_Port(uint16_t port)
    {
        acceptor->Bind(port);
    }
    void loop();
    void updateChannel(Channel* channel)
    {
        poller->update_channel(channel);
    }
    void RemoveChannel(Channel* channel)
    {
        poller->remove_channel(channel);
    }
    void UpdateNewConnection(TcpConnection* t)
    {
        Reg_connect[t->getSock()->getfd()]=t;
    }
    void Remove_Connection(TcpConnection* t)
    {
        int fd=t->getSock()->getfd();
        Reg_connect.erase(fd);
        poller->remove_channel(fd);
    }
    void runAfter(Channel::functor &func,float time);
    void stop(){
        looping=false;
        poller->Wakeup();
    }
private:
    std::map<float,timer*> timer_tree;
    std::unordered_map<int,TcpConnection*> Reg_connect;
    Poller* poller;
    Acceptor* acceptor;
    bool looping=false;
};
#endif
