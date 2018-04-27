#include "MUTEXLOCK.hpp"
#include <assert.h>
MutexLock::MutexLock():islocking_(false)
{
    assert(!pthread_mutex_init(&mutex_,NULL));
}
MutexLock::~MutexLock()
{
  assert(!islocking_);
  assert(!pthread_mutex_destroy(&mutex_));
}
void MutexLock::lock()
{

  assert(!pthread_mutex_lock(&mutex_));
  islocking_=true;
}
void MutexLock::unlock()
{
  islocking_=false;
  assert(!pthread_mutex_unlock(&mutex_));
}
