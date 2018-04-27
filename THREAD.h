#ifndef _THREAD_H
#define _THREAD_H
#include <pthread.h>
#include <boost/noncopyable.hpp>
#include "MUTEXLOCK.hpp"
#include "CONDITION.hpp"
#include <iostream>
#include <functional>
#include <assert.h>
class Thread
{
typedef std::function<void ()> threadFunc;
public:
  Thread(threadFunc& func_);
  virtual ~Thread();
  void start();
  void join();
  pthread_t getThreadid() const{return threadId_;}
  bool isrunning(){return isRunning_;}
private:
  static void *runInthread(void* arg);
  pthread_t threadId_;
  bool isRunning_;
  threadFunc Func_;
};
#endif
