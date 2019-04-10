#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include <map>
#include <vector>
#include <unordered_map>
#include <jemalloc/include/jemalloc/jemalloc.h>
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
class TcpServer;
class UdpServer;
class EventLoop
{
public:
    typedef std::shared_ptr<Channel> ChanPTR;
    EventLoop(int maxevents=1024,TcpServer* ts=nullptr);
    EventLoop(int maxevents=1024,UdpServer* us=nullptr);
    ~EventLoop();
    //nocopyable
    EventLoop(const EventLoop& other)=delete;
    EventLoop& operator =(const EventLoop& other)=delete;
    //The core function ,witch loops through the events on the epoll descriptor
    //and processes them according to the user-registered callback function.
    void loop();
    //Modify an existing channel or regist a new channel
    void updateChannel(Channel* channel)
    {
        poller->update_channel(channel);
    }
    //Remove an existing channel
    void RemoveChannel(Channel* channel)
    {
        poller->remove_channel(channel);
    }
    void RemoveChannel(int fd)
    {
        poller->remove_channel(fd);
    }
    //Regist a new connection to this loop
    //Register a timer with the eventloop and specify its callback function and time
    void runAfter(Channel::functor &func,float time);
    void runEvery(Channel::functor &func,float time);
    //End loop, wake up poller
    void stop(){
        looping=false;
        poller->Wakeup();
    }
private:
    //The following objects are owned by eventloop:a RB tree of timer file descriptor,a hash table that manage all the connection
    //accepted by this port,a poller that can manage an IO-multiplexing file descriptor,an acceptor binded on the user's port.
    std::map<float,timer*> timer_tree;
    union ServerPtr{
    TcpServer* Tcpserver_;
    UdpServer* Udpserver_;
    };
    //0:Udp 1:Tcp
    int8_t protocol;
    ServerPtr server_;
    Poller* poller;
    bool looping=false;
};
#endif
