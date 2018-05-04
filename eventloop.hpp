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
    EventLoop();
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
    pthread_t pt;
};
#endif // EVENTLOOP_HPP
