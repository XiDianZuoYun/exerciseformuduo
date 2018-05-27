#include "timer.h"

timer::timer():timerfd(timerfd_create(CLOCK_MONOTONIC,0))
{
    assert(timerfd!=-1);
}
void timer::Init(long seconds, bool iscontinue)
{
    long n_sec=seconds-(int)seconds;
    long sec=(float)((int)seconds);
    itimerspec new_value,old_value;
    new_value.it_value.tv_nsec=n_sec*10000000;
    new_value.it_value.tv_sec=sec;
    if(iscontinue)
    {
        new_value.it_interval.tv_sec=new_value.it_value.tv_sec;
        new_value.it_interval.tv_nsec=new_value.it_value.tv_nsec;
    }
    assert(timerfd_settime(timerfd,0,&new_value,&old_value)==0);
    assert(clock_gettime(CLOCK_MONOTONIC,&timeclock)==0);
    timeclock.tv_nsec+=n_sec;
    timeclock.tv_sec+=sec;
}
void timer::updateTimer()
{
    itimerspec temp;
    assert(timerfd_gettime(timerfd,&temp)==0);
    assert(clock_gettime(CLOCK_MONOTONIC,&timeclock)==0);
    timeclock.tv_nsec+=temp.it_value.tv_nsec;
    timeclock.tv_sec+=temp.it_value.tv_sec;
}
void timer::stopTimer()
{
    assert(close(timerfd)==0);
}
timer::~timer()
{
    stopTimer();
}
