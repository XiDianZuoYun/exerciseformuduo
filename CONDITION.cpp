#include <pthread.h>
#include <assert.h>
#include "CONDITION.hpp"
condition::condition(MutexLock& mutex_):MUTEX_(mutex_)
{
  assert(!pthread_cond_init(&cond_,NULL));
}
condition::~condition()
{
  assert(!pthread_cond_destroy(&cond_));
}
void condition::wait()
{
  assert(MUTEX_.islocking());
//  MUTEX_.lock();
  MUTEX_.clearflag();
  pthread_cond_wait(&cond_,MUTEX_.getMutexptr());
  MUTEX_.restoreMutex();
}
void condition::notify()
{
  assert(!pthread_cond_signal(&cond_));
}
void condition::notifyAll()
{
  assert(!pthread_cond_broadcast(&cond_));
}
