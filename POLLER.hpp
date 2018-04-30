#ifndef poller_
#define poller_
#include <unistd.h>
#include <memory>
#include <sys/epoll.h>
#include <map>
#include <assert.h>
#include <vector>
#include "MUTEXLOCK.hpp"
#include "Channel.hpp"
#define EVENTOUT EPOLLOUT
#define EVENTIN EPOLLIN
#define EVENTERR EPOLLERR
class Channel;
class Poller{
public:
    enum command_list{
        wakeup=0,
        sendsocket=1,
        breakdown=2
    };
    typedef struct command
    {
        command_list commandflag;
        void* data;
        command() {}
    } command;
  Poller(int maxsize);
  ~Poller();
  void updateChannel(Channel* _channel);
  void waitforevents(std::vector<Channel*>& activeChannels_);
  void removeChannel(Channel* _channel);
  void AwakePoller(command* cmd);
private:
  std::map<int,Channel*> events;
  std::vector<epoll_event> revents_;
  int epfd_;
  int handlenums;
  int awakefd[2];
  MutexLock lock_;
  //vector<Channel> activeChannels_;
};
#endif
