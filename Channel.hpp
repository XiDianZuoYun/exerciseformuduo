#ifndef CHANNEL
#define CHANNEL
#include <functional>
#include <memory>
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
  Channel(int fd,unsigned int events,int buffersize,EventLoop* _loop):fd_(fd),regevents(events),
                loop(_loop),buf(new Buffer(buffersize))
  {
        functor readfunc=std::bind(&Channel::readExample,*this);
        readCallback=readfunc;
        functor writefunc=std::bind(&Channel::writeExample,*this);
        writeCallback=writefunc;
  }
  ~Channel()
  { }
  void setevents(int events){events_=events;}
  void addevents(int events){events_|=events;}
  void handleEvent();
  std::vector<functor> getCallBack();
  int getfd();
  std::shared_ptr<Buffer> getbuf(){return buf;}
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
  unsigned int events_;
  unsigned int regevents;
  EventLoop* loop;
  std::shared_ptr<Buffer> buf;
  functor readCallback;
  functor writeCallback;
  functor closeCallback;
  functor errorCallback;
  bool handling_;
};
#endif
