#include "channel.h"

Channel::Channel(int fd_, int reg_events):fd_(fd_),events_(reg_events),
  readCallback(NULL),writeCallback(NULL),errorCallback()
{

}
void Channel::enable_read()
{
    events_|=EPOLLIN;
    update();
}
void Channel::enable_write()
{
    events_|=EPOLLOUT;
    update();
}
void Channel::update()
{
    std::cout<<"update"<<std::endl;
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
    if(r_events&EPOLLET)
    {
        std::cout<<"error"<<std::endl;
        errorCallback();
    }
}
