#ifndef CHANNEL_H
#define CHANNEL_H
#include <sys/epoll.h>
#include <functional>
#include <iostream>
class Channel
{
public:
    typedef std::function<void()> functor;
    Channel(int fd_,int reg_events);
    void enable_read();
    void enable_write();
    void setreadCallback(functor &func){readCallback=func;}
    void setWriteCallback(functor &func){writeCallback=func;}
    void setErrorCallback(functor &func){errorCallback=func;}
    void update();
    void handle();
    int getfd(){return fd_;}
    void setevents(int events__){r_events=events__;}
private:
    int fd_;
    int events_;
    int r_events;
    functor readCallback;
    functor writeCallback;
    functor errorCallback;
};

#endif // CHANNEL_H
