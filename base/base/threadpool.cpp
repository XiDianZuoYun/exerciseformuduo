#include "threadpool.h"
ThreadPool::ThreadPool():isplaying(false),quit(false),mutex_(),cond_t()
{
    for(int i=0;i<MAXTHREAD;++i)
        pthread_create(&workqueue[i],NULL,Thread_run,this);
}
ThreadPool::~ThreadPool()
{
    Quit();
}
void ThreadPool::Start()
{
    if(isplaying)
        return;
    isplaying=true;
    cond_t.notify_all();
}
void ThreadPool::Stop()
{
    if(!isplaying)
        return;
    isplaying=false;
    cond_t.notify_all();
}
void ThreadPool::Quit()
{
    this->quit=true;
    this->isplaying=false;
    cond_t.notify_all();
}
void ThreadPool::push_task(Task::task_func &tf, void *data)
{
    std::lock_guard<std::mutex> mtx(mutex_);
    taskqueue.emplace(tf,data);
    cond_t.notify_one();
}
void ThreadPool::run_task()
{
    while(!quit){
        while(!isplaying)
            usleep(100);
        Task t=Get_Task();
        if(!isplaying)
             break;
        t.func(t.data);
    }
}
Task ThreadPool::Get_Task()
{
    std::unique_lock<std::mutex> mtx(mutex_);
    while(taskqueue.empty()&&isplaying)
        cond_t.wait(mtx);
    Task t=taskqueue.front();
    taskqueue.pop();
    mtx.unlock();
    return t;
}
