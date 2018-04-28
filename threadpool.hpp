#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP
#include <functional>
#include "MUTEXLOCK.hpp"
#include "CONDITION.hpp"
#include "THREAD.h"
#include "CONDITION.hpp"
#include "MUTEXLOCK.hpp"
#include <queue>
#include <vector>
#define maxthreadsize 16
class ThreadPool{
public:
    typedef std::function<void*(void*)> functor;
    typedef struct task
    {
        functor func;
        void* arg;
        task() {}
    } threadtask;
    bool init();
    void quit();
    void addtask(const ThreadPool::threadtask& _task);
    void startpool();
    ThreadPool(ThreadPool& thredpool)=delete;
    ThreadPool operator =(ThreadPool& threadpool)=delete;
    ThreadPool():isrunning_(false),quit_(false),mutex_(),cont_(mutex_){_init();}

private:
    void _init();
    void thread_process();
    threadtask take();
    std::function<void ()> _func;
    std::queue<threadtask> taskpool;
    std::vector<Thread> m_thrs;
    bool isrunning_;
    bool quit_;
    MutexLock mutex_;
    condition cont_;
};
#endif // THREADPOOL_HPP
