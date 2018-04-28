#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP
#include <pthread.h>
#include <vector>
#include "POLLER.hpp"
#include "Channel.hpp"
class Poller;

class EventLoop{
public:
    EventLoop(Poller* _poller);
    bool isInloopThread();
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
