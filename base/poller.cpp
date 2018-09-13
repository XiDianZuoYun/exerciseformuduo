#include "poller.h"
#define POLLER_DEBUG
Poller::Poller():epoll_fd(epoll_create(1024)),reg_nums(0),isPolling_(false)
{
    //Use socketpair to be the file description that can wake poller up.
    //If we want to wake poller up,we just need to write one byte into socketpair.
    assert(socketpair(AF_UNIX,SOCK_DGRAM,0,wake_fd)>=0);
    epoll_event temp;
    bzero(&temp,sizeof(epoll_event));
    temp.data.fd=wake_fd[0];
    temp.events|=EPOLLIN;
    temp.events|=EPOLLET;
    assert(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,wake_fd[0],&temp)==0);
}
//This function can only be called by EventLoop.
Poller::~Poller()
{
    close(epoll_fd);
    close(wake_fd[0]);
    close(wake_fd[1]);
}
void Poller::remove_channel(ChannelPtr _channel)
{
    int fd=_channel->getfd();
    auto iter=reg_Channel.find(fd);
    if(iter==reg_Channel.end())
        std::cout<<"No channel in this map!";
    reg_Channel.erase(iter);
    assert(epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,nullptr)==0);
    reg_nums--;
}
void Poller::update_channel(ChannelPtr _channel)
{
    int fd=_channel->getfd();
    auto iter=reg_Channel.find(fd);
    if(iter==reg_Channel.end())
    {
        epoll_event temp;
        bzero(&temp,sizeof(epoll_event));
        temp.events|=EPOLLET;
        temp.events|=_channel->getevents();
        temp.data.ptr=(void*)_channel.get();
        reg_Channel[fd]=_channel;
        assert(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&temp)==0);
        reg_nums++;
        if(return_events.capacity()<=reg_nums)
            return_events.resize(reg_nums*2);
    }else
    {
        if(_channel->getevents()==(iter->second->getevents()))
            return;
        reg_Channel[fd]=_channel;
        epoll_event temp;
        bzero(&temp,sizeof(epoll_event));
        temp.events|=EPOLLET;
        temp.events|=_channel->getevents();
        temp.data.ptr=(void*)_channel.get();
        assert(epoll_ctl(epoll_fd,EPOLL_CTL_MOD,fd,&temp)==0);
    }
}
void Poller::update_channel(Channel *_channel)
{
    ChannelPtr temp(_channel);
    update_channel(temp);
}
#ifdef POLLER_DEBUG
void Poller::AddFD(int fd)
{
    epoll_event temp;
    bzero(&temp,sizeof(epoll_event));
    temp.data.fd=fd;
    temp.events|=(EPOLLIN|EPOLLET);
    assert(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&temp)==0);
}
void Poller::Poll_debug()
{
    epoll_event epoll_array[20];
    int n=epoll_wait(epoll_fd,epoll_array,20,-1);
    for(int i=0;i<n;++i)
        std::cout<<epoll_array[i].data.fd<<' '<<epoll_array[i].events<<std::endl;
}
#endif
void Poller::remove_channel(Channel *_channel)
{
    ChannelPtr temp(_channel);
    remove_channel(temp);
}
void Poller::poll(std::vector<ChannelPtr> &active)
{
    if(reg_nums==0)
        return;
    assert(!isPolling_);
    isPolling_=true;
    int n=epoll_wait(epoll_fd,&(*return_events.begin()),reg_nums,-1);
    assert(n>=0);
    for(int i=0;i<n;++i)
    {
        auto& event=return_events[i];
        if(event.data.fd==wake_fd[0])
            std::cout<<"wake up!"<<std::endl;
        else{
            Channel* ch=(Channel*)event.data.ptr;
            ch->setevents(event.events);
            active.emplace_back(ch);
        }
    }
}
void Poller::Wakeup()
{
    if(!isPolling_)
    {
        std::cout<<"Poller is not blocking!";
        return;
    }
    char c='1';
    assert(write(wake_fd[1],(void*)&c,1)==1);
    return;
}
