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
#define POLLER_DEBUG
class Poller
{
private:
    bool isPolling_;
    typedef std::shared_ptr<Channel> ChannelPtr;
    std::map<int,ChannelPtr> reg_Channel;
    int epoll_fd;
    int wake_fd[2];
    int reg_nums;
    std::vector<epoll_event> return_events;
    //EventLoop* loop
public:
    Poller();
    ~Poller();
    void poll(/*timeclock* sec,*/std::vector<ChannelPtr>& active);
    void remove_channel(ChannelPtr _channel);
    void update_channel(ChannelPtr _channel);
#ifdef POLLER_DEBUG
    void AddFD(int fd);
    void removeFD(int fd);
    void Poll_debug();
#endif
    void remove_channel(Channel* _channel);
    void update_channel(Channel* _channel);
    void Wakeup();
};

#endif // POLLER_H
