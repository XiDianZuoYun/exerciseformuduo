#ifndef poller_
#define poller_
#include <sys/epoll.h>
#include <map>
#include <assert.h>
#include <vector>
#include "MUTEXLOCK.hpp"
#include <Channel.hpp>
#include <unistd.h>
#include <memory>
#define EVENTOUT EPOLLOUT
#define EVENTIN EPOLLIN
#define EVENTERR EPOLLERR
class Channel;
class Poller{
public:
  Poller(int maxsize);
  ~Poller();
  void updateChannel(Channel* _channel);
  void waitforevents(std::vector<Channel*>& activeChannels_);
  void removeChannel(Channel* _channel);
private:
  std::map<int,Channel*> events;
  std::vector<epoll_event> revents_;
  int epfd_;
  int handlenums;
  MutexLock lock_;
  //vector<Channel> activeChannels_;
};
#endif
