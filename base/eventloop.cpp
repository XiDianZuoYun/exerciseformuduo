#include "eventloop.h"
#include "tcpserver.h"
#include <iostream>
static __thread EventLoop* _loop=nullptr;
#ifndef ALLOCATE(TYPE)
#define ALLOCATE(TYPE) new(malloc(sizeof(TYPE)))
#endif
EventLoop::EventLoop(int maxevents, TcpServer *ts):poller(ALLOCATE(Poller) Poller(maxevents,this)),Tcpsever_(ts)
{
    assert(_loop==nullptr);
    _loop=this;
    assert((poller!=nullptr));
}
EventLoop::~EventLoop() noexcept
{
    delete poller;
    for(auto &t:timer_tree)
        delete t.second;
}
void EventLoop::loop()
{
    if(!looping)
        looping=true;
    while (looping) {
        std::vector<Channel*> active;
        poller->poll(active);
        for(auto ptr:active){
            std::cout<<"active!";
            assert(ptr!=nullptr);
            ptr->handle();
        }
        timespec current;
        clock_gettime(CLOCK_MONOTONIC,&current);
        float cur_time=static_cast<float>(current.tv_sec+current.tv_nsec*0.000000001);
        if(timer_tree.empty())
            continue;
        for(auto &timer:timer_tree)
        {
            assert(timer.second!=nullptr);
            if(timer.first>cur_time)
                break;
            timer.second->updateTimer();
            if(timer.second->getTime()<=cur_time)
                timer_tree.erase(timer.first);
            else
                timer_tree[timer.second->getTime()]=timer.second;
        }
        if(Tcpsever_!=nullptr)
            Tcpsever_->Clear_connection();
    }
}
void EventLoop::runAfter(Channel::functor &func,float time)
{
    timer* t=new timer(this);
    t->Init(time,false);
    timespec cur_t;
    clock_gettime(CLOCK_MONOTONIC,&cur_t);
    float current_time=static_cast<float>(cur_t.tv_sec+cur_t.tv_nsec*0.000000001);
    timer_tree[current_time+time]=t;
    t->getChannel()->setreadCallback(func);
    poller->update_channel(t->getChannel());
}
void EventLoop::runEvery(Channel::functor &func, float time)
{
    timer* t=new timer(this);
    t->Init(time,true);
    timespec cur_t;
    clock_gettime(CLOCK_MONOTONIC,&cur_t);
    float current_time=static_cast<float>(cur_t.tv_sec+cur_t.tv_nsec*0.000000001);
    timer_tree[current_time+time]=t;
    t->getChannel()->setreadCallback(func);
    poller->update_channel(t->getChannel());
}
