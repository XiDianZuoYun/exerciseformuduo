#ifndef TIMER_H
#define TIMER_H
#include <unistd.h>
#include <sys/timerfd.h>
#include <assert.h>
#include <functional>
class timer
{
public:
    typedef std::function<void()> functor;
    timer();
    ~timer();
    timer(const timer& t)=delete;
    timer& operator=(const timer& t)=delete;
    void setCallback(functor& cb){timercallback=cb;}
    float getTime(){return timeclock.tv_sec+timeclock.tv_nsec*0.000000001;}
    int getfd(){return timerfd;}
    void Init(float seconds,bool iscontinue);
    void updateTimer();
    void stopTimer();
    void handle(){timercallback();}
private:
    functor timercallback;
    int timerfd;
    timespec timeclock;
    timespec repeatclock;
};

#endif // TIMER_H
