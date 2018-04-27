#include "threadpool.hpp"
#include <functional>
void ThreadPool::thread_process()
{
    assert(!isrunning_);
    while(!quit_&&isrunning_)
    {
      threadtask task=take();
      if(task.arg!=NULL)
          task.func(task.arg);
    }
    if(quit_)
        return;
}

void ThreadPool::_init()
{
    std::function<void()> _func=std::bind(&ThreadPool::thread_process,this);
    Thread temp(_func);
    for(int i=0;i<maxthreadsize;++i)
        m_thrs.push_back(temp);
}
void ThreadPool::addtask(const ThreadPool::threadtask& _task)
{
    mutex_.lock();
    taskpool.push(_task);
    cont_.notify();
    mutex_.unlock();
}
ThreadPool::threadtask ThreadPool::take()
{
    MutexLockGuard __lock(mutex_);
    while(taskpool.empty()&&isrunning_)
    {
        cont_.wait();
    }
    ThreadPool::threadtask task;
    if(!taskpool.empty())
    {
        task=taskpool.front();
        taskpool.pop();
    }
    return task;
}
void ThreadPool::startpool()
{
    assert(!isrunning_);
    isrunning_=true;
    for(int i=0;i<m_thrs.size();++i)
    {
        if(m_thrs[i].isrunning())
            continue;
        m_thrs[i].start();
    }
}
void ThreadPool::quit()
{
    mutex_.lock();
    if(isrunning_)
        isrunning_=false;
    quit_=true;
    cont_.notifyAll();
    mutex_.unlock();
}
