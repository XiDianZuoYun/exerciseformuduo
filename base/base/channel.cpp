#include "channel.h"
#include "eventloop.h"
Channel::Channel(int fd_, unsigned int reg_events, EventLoop* lp,int8_t Ch_type):fd_(fd_),
    events_(reg_events),loop(lp),type(Ch_type)
{

}
Channel::~Channel()
{
    if(loop!=nullptr)
        loop->RemoveChannel(this);
}
void Channel::enable_read()
{
    events_|=EPOLLIN;
    update();
}
void Channel::disable_read()
{
    events_&=(~EPOLLIN);
    update();
}
void Channel::enable_write()
{
    events_|=EPOLLOUT;
    update();
}
void Channel::disable_write()
{
    events_&=(~EPOLLOUT);
    update();
}
void Channel::update()
{
    std::cout<<"update"<<std::endl;
    loop->updateChannel(this);
}
void Channel::handle()
{
    if(r_events&EPOLLIN)
    {
        std::cout<<"read!"<<std::endl;
        readCallback();
    }
    if(r_events&EPOLLOUT)
    {
        std::cout<<"write!"<<std::endl;
        writeCallback();
    }
    if(r_events&EPOLLERR)
    {
        std::cout<<"error"<<std::endl;
        errorCallback();
    }
}
void Channel::unregistered()
{
    if(!loop)
        return;
    loop->RemoveChannel(this);
}
