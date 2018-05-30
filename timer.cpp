#include "timer.h"

timer::timer():timerfd(timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK))
{
    assert(timerfd!=-1);
}
void timer::Init(float seconds, bool iscontinue)
{
    float n_sec=seconds-(int)seconds;
    long sec=(long)((int)seconds);
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
    assert(timerfd_settime(timerfd,0,&new_value,NULL)==0);
    assert(clock_gettime(CLOCK_MONOTONIC,&timeclock)==0);
    timeclock.tv_nsec+=n_sec;
    timeclock.tv_sec+=sec;
}
void timer::updateTimer()
{
    timeclock.tv_nsec+=repeatclock.tv_nsec;
    timeclock.tv_sec+=repeatclock.tv_sec;
}
void timer::stopTimer()
{
    assert(close(timerfd)==0);
}
timer::~timer()
{
    stopTimer();
}
