#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP
#include <pthread.h>
#include <vector>
#include <map>
#include "POLLER.hpp"
#include "Channel.hpp"
#include "MUTEXLOCK.hpp"
class Channel;
class Poller;
class EventLoop{
public:
    EventLoop(Poller* _poller);
    bool isInloopThread();
    void updateChannel(Channel* _chan);
    ~EventLoop();
    void loop();
    void quit();
private:
    bool looping_;
    bool quit__;
    std::vector<Channel*>* activeChannels_;
    Poller *poller;
    pid_t pt;
};
#endif // EVENTLOOP_HPP
