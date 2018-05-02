#ifndef poller_
#define poller_
#include <unistd.h>
#include <memory>
#include <sys/epoll.h>
#include <map>
#include <assert.h>
#include <vector>
#include "MUTEXLOCK.hpp"
#include <functional>
#include "Channel.hpp"
#define EVENTOUT EPOLLOUT
#define EVENTIN EPOLLIN
#define EVENTERR EPOLLERR
class Channel;
class Poller{
public:
  Poller(int maxsize);
  ~Poller();
  void updateChannel(Channel* _channel);
  void delegate(std::function<void()> &cb){pendingwork=cb;}
  void waitforevents(std::vector<Channel*>& activeChannels_);
  void removeChannel(Channel* _channel);
  void AwakePoller();
private:
  std::map<int,Channel*> events;
  std::vector<epoll_event> revents_;
  std::function<void()> pendingwork;
  int epfd_;
  int handlenums;
  int awakefd[2];
  MutexLock lock_;
  //vector<Channel> activeChannels_;
};
#endif
