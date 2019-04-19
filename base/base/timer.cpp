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
void timer::RunAt(std::string &date, std::string &time, unsigned int ms)
{
    assert(ms<100);
    struct tm t;
    bzero(&t,sizeof(struct tm));
    sscanf(date.c_str(),"%d-%d-%d",&t.tm_year,&t.tm_mon,&t.tm_mday);
    sscanf(time.c_str(),"%d:%d:%d",&t.tm_hour,&t.tm_min,&t.tm_sec);
    time_t ret=mktime(&t);
    assert(ret!=-1);
    float seconds=ret+ms/100;
    Init(seconds,false);
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
