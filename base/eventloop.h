#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include <map>
#include <vector>
#include <unordered_map>
#include <jemalloc/jemalloc.h>
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
    //nocopyable
    EventLoop(const EventLoop& other)=delete;
    EventLoop& operator =(const EventLoop& other)=delete;
    void setDefaultCallback(TcpConnection::CallBack c)
    {
        acceptor->setCallBack(c);
    }
    void Bind_Port(uint16_t port)
    {
        acceptor->Bind(port);
    }
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
    //Regist a new connection to this loop
    void UpdateNewConnection(TcpConnection* t)
    {
        Reg_connect[t->getSock()->getfd()]=t;
    }
    //Remove an existing channel
    void Remove_Connection(TcpConnection* t)
    {
        int fd=t->getSock()->getfd();
        auto it=Reg_connect.find(fd);
        if(it==Reg_connect.end())
            return;
        Reg_connect.erase(it);
        poller->remove_channel(fd);
    }
    //Register a timer with the eventloop and specify its callback function and time
    void runAfter(Channel::functor &func,float time);
    //End loop, wake up poller
    void stop(){
        looping=false;
        poller->Wakeup();
    }
private:
    //The following objects are owned by eventloop:a RB tree of timer file descriptor,a hash table that manage all the connection
    //accepted by this port,a poller that can manage an IO-multiplexing file descriptor,an acceptor binded on the user's port.
    std::map<float,timer*> timer_tree;
    std::unordered_map<int,TcpConnection*> Reg_connect;
    Poller* poller;
    Acceptor* acceptor;
    bool looping=false;
};
#endif
