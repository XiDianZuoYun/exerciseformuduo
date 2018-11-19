#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <pthread.h>
#include <unistd.h>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
constexpr int MAXTHREAD=4;
typedef struct task__
{    
public:
    typedef std::function<void(void*)> task_func;
    task__(task_func& tf,void* data__):func(tf),data(data__)
    {}
    task__& operator=(const task__& other)=default;
    task_func func;
    void* data;
} Task;
class ThreadPool
{
public:
    using task_func=Task::task_func;
    ThreadPool();
    ~ThreadPool();
    void Start();
    void Stop();
    void Quit();
    void push_task(task_func& tf,void* data);
private:
    pthread_t workqueue[MAXTHREAD];
    std::mutex mutex_;
    std::condition_variable cond_t;
    static void* Thread_run(void* arg)
    {
        ThreadPool* tp=static_cast<ThreadPool*>(arg);
        tp->run_task();
        return static_cast<void*>(0);
    }
    void run_task();
    Task Get_Task();
    std::queue<Task> taskqueue;
    bool isplaying;
    bool quit;
};

#endif // THREADPOOL_H
