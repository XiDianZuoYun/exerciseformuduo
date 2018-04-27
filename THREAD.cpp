#include "THREAD.h"
Thread::Thread(threadFunc& func_):isRunning_(false),threadId_(0),Func_(func_){}
Thread::~Thread()
{
  if(isRunning_)
  {
    assert(!pthread_detach(threadId_));
  }
}
void *Thread::runInthread(void *arg)
{
  Thread* pt=static_cast<Thread*>(arg);
  pt->Func_();
  return NULL;
}
void Thread::start()
{
  assert(!pthread_create(&threadId_,NULL,Thread::runInthread,this));
  isRunning_=true;
}
void Thread::join()
{
  assert(isRunning_);
 assert(!pthread_join(threadId_,NULL));//回收线程
 isRunning_=false;
}
