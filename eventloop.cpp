#include <pthread.h>
#include "eventloop.hpp"
class EventLoop;
__thread EventLoop* loopinthisThread=0;
EventLoop::EventLoop(Poller* _poller):poller(_poller),looping_(false),quit__(false)
{
    assert(!loopinthisThread);
    loopinthisThread=this;
    pt=::pthread_self();
}
EventLoop::~EventLoop()
{
    assert(!looping_);
    poller->~Poller();
    std::vector<Channel*> empty;
    for(auto it:*activeChannels_)
        it->~Channel();
    activeChannels_->clear();
    activeChannels_->swap(empty);
}
bool EventLoop::isInloopThread()
{
    return pt==::pthread_self();
}
void EventLoop::updateChannel(Channel *_chan)
{
    poller->updateChannel(_chan);
}
void EventLoop::loop()
{
    typedef std::vector<Channel*>::iterator iterator;
    looping_=true;
    assert(isInloopThread());
    while(!quit__)
    {
        activeChannels_->clear();
        poller->waitforevents(*activeChannels_);
        for(iterator it=(*activeChannels_).begin();it!=(*activeChannels_).end();++it)
            (*it)->handleEvent();
    }
}
void EventLoop::quit()
{
    assert(isInloopThread());
    looping_=false;
    quit__=true;
}
