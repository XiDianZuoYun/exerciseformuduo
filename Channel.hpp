#ifndef CHANNEL
#define CHANNEL
#include <functional>
#include <iostream>
#include "POLLER.hpp"
#include <vector>
class Channel{
public:
  typedef std::function<void()> functor;
  Channel(){}
  Channel(int fd,int events):fd_(fd),regevents(events){}
  ~Channel(){}
  void setevents(int events){events_=events;}
  void addevents(int events){events_|=events;}
  void handleEvent();
  std::vector<functor> getCallBack();
  int getfd();
  void setreg(int newevent){regevents=newevent;}
  int returnreg(){return regevents;}
  bool ishandling_();
  void usinghandling_(){handling_=true;}
  int getIdx();
  void setReadCallback(functor& cb){readCallback=cb;}
  void setWriteCallback(functor& cb){writeCallback=cb;}
  void setErrorCallback(functor& cb){errorCallback=cb;}
  void setCloseCallback(functor& cb){closeCallback=cb;}
  void setReadCallback(functor&& cb){readCallback=std::move(cb);}
  void setWriteCallback(functor&& cb){writeCallback=std::move(cb);}
  void setErrorCallback(functor&& cb){errorCallback=std::move(cb);}
  void setCloseCallback(functor&& cb){closeCallback=std::move(cb);}
private:
  int fd_;
  int events_;
  int regevents;
  functor readCallback;
  functor writeCallback;
  functor closeCallback;
  functor errorCallback;
  bool handling_;
};
#endif
