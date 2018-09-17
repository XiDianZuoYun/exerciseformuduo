#ifndef TIMER_H
#define TIMER_H
#include <unistd.h>
#include <sys/timerfd.h>
#include <assert.h>
#include <functional>
#include "channel.h"
class EventLoop;
class timer
{
public:
    timer(EventLoop* lp=nullptr);
    ~timer();
    timer(const timer& t)=delete;
    timer& operator=(const timer& t)=delete;
    float getTime(){return timeclock.tv_sec+timeclock.tv_nsec*0.000000001;}
    int getfd(){return timerfd;}
    void Init(float seconds,bool iscontinue);
    inline void updateTimer()
    {
        timeclock.tv_nsec+=repeatclock.tv_nsec;
        timeclock.tv_sec+=repeatclock.tv_sec;
    }
    inline void updateTimer(float cur_time)
    {
        if(getTime()<=cur_time)
        {
            timeclock.tv_nsec=static_cast<long>(cur_time-static_cast<int>(cur_time));
            timeclock.tv_nsec+=repeatclock.tv_nsec;
            timeclock.tv_sec=static_cast<long>(cur_time);
            timeclock.tv_sec+=repeatclock.tv_sec;
        }
    }
    void stopTimer();
    void handle(){timer_channel->handle();}
    Channel* getChannel() const{return timer_channel;}
private:
    int timerfd;
    timespec timeclock;
    timespec repeatclock;
    Channel* timer_channel;
    EventLoop* loop;
};

#endif // TIMER_H
