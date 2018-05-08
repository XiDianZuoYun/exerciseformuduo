#include <sys/epoll.h>
#include <iostream>
#include <map>
#include <vector>
#include "POLLER.hpp"
/*Before we use the poller,it's neccessary to make sure that we have power to let it return.
Variable handlenmus is the number of channels registered in this poller.*/
Poller::Poller(int maxsize)
{
  assert((epfd_=epoll_create(maxsize))>0);
  assert(!pipe(awakefd));
  epoll_event temp;
  temp.data.fd=awakefd[0];
  temp.events|=EPOLLIN;
  temp.events|=EPOLLET;
  if(epoll_ctl(epfd_,EPOLL_CTL_ADD,awakefd[0],&temp)<0)
      std::cout<<errno<<std::endl;
  handlenums=1;
}
/*We assume that the epoll file description will be managed by the only poller.if the poller is closed,
the file description will be alse closed.*/
Poller::~Poller()
{
  close(epfd_);
  typedef std::map<int,Channel*>::iterator iterator;
  for(iterator it=events.begin();it!=events.end();++it)
  {
      (*it).second->~Channel();
  }
  events.clear();
}
/*Write in pipe,this wil make wait function return*/
void Poller::AwakePoller()
{
   char a='1';
   assert(write(awakefd[1],(void*)&a,sizeof(a))>0);

}
/*According to user's command,update Channel list.
If the new channel can be found in map,its register event will be updated.*/
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
        assert(!epoll_ctl(epfd_,EPOLL_CTL_MOD,_channel->getfd(),&temp));
    }
    else{
        events[_channel->getfd()]=_channel;
        epoll_event temp;
        temp.data.fd=_channel->getfd();
        temp.events=_channel->returnreg();
        temp.events|=EPOLLET;
        handlenums+=1;
        if(revents_.size()<=handlenums)
            revents_.resize(2*handlenums);
        assert(!epoll_ctl(epfd_,EPOLL_CTL_ADD,_channel->getfd(),&temp));
    }
}
/*blocked on epoll_wait,unless waked by wakefd or events happened.
Then poller will push all of the active channels into the IO thread's loop.*/
void Poller::waitforevents(std::vector<Channel*> &activeChannels_)
{
  typedef std::map<int,Channel*>::iterator iterator;
  for (iterator it=events.begin();it!=events.end();++it) {
    (*it).second->usinghandling_();
  }
  std::cout<<"wait"<<std::endl;
  int n=epoll_wait(epfd_,&(*revents_.begin()),handlenums,-1);
  if(errno!=0)
    std::cerr<<"error:"<<errno<<std::endl;
  std::cout<<n<<std::endl;
  for(int i=0;i<n;++i)
  {
    std::cout<<"fill"<< n<<std::endl;
    int index=revents_[i].data.fd;
    int revent_=revents_[i].events;
    if(index!=awakefd[0])
    {
        events[index]->setevents(revent_);
        MutexLockGuard __lock(lock_);
        Channel* temp=events[index];
        activeChannels_.push_back(std::move(temp));
    }
    if(pendingwork)
        pendingwork();
    epoll_event temp;
    for(iterator it=events.begin();it!=events.end();++it)
    {
        Channel* ch=(*it).second;
        temp.data.fd=ch->getfd();
        temp.events=ch->returnreg();
        assert(!epoll_ctl(epfd_,EPOLL_CTL_MOD,ch->getfd(),&temp));
    }
  }
}
/*remove that has been registered in this poller.*/
void Poller::removeChannel(Channel* _channel)
{
  typedef std::map<int,Channel*>::iterator iterator;
  iterator it=events.find(_channel->getfd());
  assert(it!=events.end());
  assert(!_channel->ishandling_());
  assert(epoll_ctl(epfd_,EPOLL_CTL_DEL,_channel->getfd(),NULL));
  events.erase(it);
}
