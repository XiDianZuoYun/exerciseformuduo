#ifndef POLLER_H
#define POLLER_H
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <map>
#include <memory>
#include <vector>
#include <assert.h>
#include <string.h>
#include "channel.h"
class EventLoop;
class Poller
{
private:
    int epoll_fd;
    bool isPolling_;
    typedef std::shared_ptr<Channel> ChannelPtr;
    std::map<int,Channel*> reg_Channel;
    int wake_fd[2];
    int reg_nums;
    std::vector<epoll_event> return_events;
    EventLoop* loop;
public:
    Poller(int maxevents=1024,EventLoop* __loop=nullptr);
    ~Poller();
    void poll(/*timeclock* sec,*/std::vector<Channel*>& active);
    void remove_channel(Channel* _channel);
    void remove_channel(int fd)
    {
        reg_Channel.erase(fd);
    }
    void update_channel(Channel* _channel);
    void Wakeup();
};

#endif // POLLER_H
