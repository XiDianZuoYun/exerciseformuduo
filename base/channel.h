#ifndef CHANNEL_H
#define CHANNEL_H
#include <sys/epoll.h>
#include <functional>
#include <iostream>
#define CHANNEL_SOCK 0x01
#define CHANNEL_TIMER 0x02
#define CHANNEL_FILE 0x04
#define CHANNEL_PIPE 0x08
class EventLoop;
class Channel
{
public:
    typedef std::function<void()> functor;
    Channel(int fd_,unsigned int reg_events,EventLoop* lp=nullptr,int8_t type=CHANNEL_SOCK);
    ~Channel();
    void enable_read();
    void enable_write();
    void setreadCallback(functor &func){readCallback=func;}
    void setWriteCallback(functor &func){writeCallback=func;}
    void setErrorCallback(functor &func){errorCallback=func;}
    void update();
    void handle();
    int getfd(){return fd_;}
    unsigned int getevents(){return events_;}
    void setevents(unsigned int events__){
        r_events=events__;
        if(r_events==EPOLLERR)
            std::cout<<"THERE IS AN ERROR!!";
    }
    void unregistered();
private:
    int fd_;
    unsigned int events_;
    unsigned int r_events;
    functor readCallback;
    functor writeCallback;
    functor errorCallback;
    EventLoop* loop;
    int8_t type;
};

#endif // CHANNEL_H
