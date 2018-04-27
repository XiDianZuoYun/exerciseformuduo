#ifndef _MUTEXLOCK_H_
#define _MUTEXLOCK_H_
#include <iostream>
#include <pthread.h>
#include <boost/noncopyable.hpp>
class MutexLock:public boost::noncopyable
{
  friend class Condition;
public:
  MutexLock();
  ~MutexLock();
  void lock();
  void unlock();
  pthread_mutex_t *getMutexptr()
  {
    return &mutex_;
  }
  bool islocking(){return islocking_;}
  void restoreMutex()
  {
    islocking_=true;
  }
  void clearflag(){islocking_=false;}
private:
  bool islocking_;
  pthread_mutex_t mutex_;
};
class MutexLockGuard:public boost::noncopyable
{
public:
  MutexLockGuard(MutexLock& mutex):mutex_(mutex){mutex_.lock();}
  ~MutexLockGuard(){mutex_.unlock();}
private:
  MutexLock &mutex_;
};
#endif
