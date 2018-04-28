#include <sys/epoll.h>
#include <iostream>
#include <map>
#include <vector>
#include "POLLER.hpp"
Poller::Poller(int maxsize)
{
  epfd_=epoll_create1(maxsize);
  assert(!pipe(awakefd));
  epoll_event temp;
  temp.data.fd=awakefd[0];
  temp.events|=EPOLLIN;
  temp.events|=EPOLLET;
  assert(!epoll_ctl(epfd_,EPOLL_CTL_ADD,awakefd[0],&temp));
  handlenums=0;
}
Poller::~Poller()
{
  close(epfd_);
  typedef std::map<int,Channel*>::iterator iterator;
  for(iterator it=events.begin();it!=events.end();++it)
  {
      (*it).second->~Channel();
  }
}
void Poller::AwakePoller()
{
    char a='1';
    assert(write(awakefd[1],(void*)&a,sizeof(a))>0);
}
void Poller::updateChannel(Channel *_channel)
{
    typedef std::map<int,Channel*>::iterator iterator;
    iterator it=events.find(_channel->getfd());
    if(it!=events.end())
    {
        events[_channel->getfd()]=_channel;
        epoll_event temp;
        temp.data.fd=_channel->getfd();
        temp.events=_channel->returnreg();
        temp.events|=EPOLLET;
        assert(!epoll_ctl(epfd_,EPOLL_CTL_ADD,_channel->getfd(),&temp));
    }
    else{
        events[_channel->getfd()]=_channel;
        epoll_event temp;
        temp.data.fd=_channel->getfd();
        temp.events=_channel->returnreg();
        temp.events|=EPOLLET;
        assert(!epoll_ctl(epfd_,EPOLL_CTL_MOD,_channel->getfd(),&temp));
    }
}
void Poller::waitforevents(std::vector<Channel*> &activeChannels_)
{
  int len=events.size();
  typedef std::map<int,Channel*>::iterator iterator;
  for (iterator it=events.begin();it!=events.end();++it) {
    (*it).second->usinghandling_();
  }
  int n=epoll_wait(epfd_,&*revents_.begin(),handlenums,-1);
  for(int i=0;i<n;++i)
  {
    int index=revents_[i].data.fd;
    int revent_=revents_[i].events;
    events[index]->setevents(revent_);
    MutexLockGuard __lock(lock_);
    Channel* temp=events[index];
    activeChannels_.push_back(std::move(temp));
  }
}
void Poller::removeChannel(Channel* _channel)
{
  typedef std::map<int,Channel*>::iterator iterator;
  iterator it=events.find(_channel->getfd());
  assert(it!=events.end());
  assert(!_channel->ishandling_());
  assert(epoll_ctl(epfd_,EPOLL_CTL_DEL,_channel->getfd(),NULL));
  events.erase(it);
}
