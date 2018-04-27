#ifndef _CONDITION_H_
#define _CONDITION_H_
#include "MUTEXLOCK.hpp"
#include <pthread.h>
#include <boost/noncopyable.hpp>
#include <assert.h>
class MutexLock;
class condition
{
public:
  condition(MutexLock &mutex);
  ~condition();
  void wait();
  void notify();
  void notifyAll();
private:
  pthread_cond_t cond_;
  MutexLock &MUTEX_;
};
#endif
