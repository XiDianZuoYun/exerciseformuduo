#include "timer.h"
#include "eventloop.h"
timer::timer(EventLoop* lp):timerfd(timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK)),
    loop(lp)
{
    assert(timerfd!=-1);
    timer_channel=new Channel(timerfd,EPOLLIN|EPOLLERR|EPOLLET,lp,CHANNEL_TIMER);
    if(lp!=nullptr)
        lp->updateChannel(timer_channel);
}
void timer::Init(float seconds, bool iscontinue)
{
    long n_sec=static_cast<long>(seconds-static_cast<int>(seconds));
    long sec=static_cast<long>(seconds);
    itimerspec new_value;
    new_value.it_value.tv_nsec=long(n_sec*10000000);
    new_value.it_value.tv_sec=sec;
    if(iscontinue)
    {
        new_value.it_interval.tv_sec=new_value.it_value.tv_sec;
        new_value.it_interval.tv_nsec=new_value.it_value.tv_nsec;
        repeatclock.tv_nsec=n_sec;
        repeatclock.tv_sec=sec;
    }else{
        new_value.it_interval.tv_sec=0;
        new_value.it_interval.tv_nsec=0;
        repeatclock.tv_nsec=n_sec;
        repeatclock.tv_sec=sec;
    }
    assert(timerfd_settime(timerfd,0,&new_value,nullptr)==0);
    assert(clock_gettime(CLOCK_MONOTONIC,&timeclock)==0);
    timeclock.tv_nsec+=n_sec;
    timeclock.tv_sec+=sec;
}
void timer::stopTimer()
{
    assert(close(timerfd)==0);
}
timer::~timer()
{
    stopTimer();
    delete timer_channel;
}
