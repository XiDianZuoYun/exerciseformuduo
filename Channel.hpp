#ifndef CHANNEL
#define CHANNEL
#include <functional>
#include <iostream>
#include <vector>
#include "POLLER.hpp"
#include "buffer.hpp"
#include "eventloop.hpp"
class EventLoop;
class Channel{
public:
  typedef std::function<void()> functor;
  Channel(){}
  Channel(int fd,int events,int buffersize,EventLoop* _loop):fd_(fd),regevents(events),
                buf(new Buffer(buffersize)),loop(_loop)
  {
        functor readfunc=std::bind(&Channel::readExample,*this);
        readCallback=readfunc;
        functor writefunc=std::bind(&Channel::writeExample,*this);
        writeCallback=writefunc;
  }
  ~Channel(){buf->~Buffer();}
  void setevents(int events){events_=events;}
  void addevents(int events){events_|=events;}
  void handleEvent();
  std::vector<functor> getCallBack();
  int getfd();
  void* getbuf(){return buf;}
  void setreg(int newevent){regevents=newevent;}
  int returnreg(){return regevents;}
  bool ishandling_();
  void usinghandling_(){handling_=true;}
  void readExample();
  void writeExample();
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
  EventLoop* loop;
  Buffer* buf;
  functor readCallback;
  functor writeCallback;
  functor closeCallback;
  functor errorCallback;
  bool handling_;
};
#endif
