#ifndef THREADPOOL_H
#define THREADPOOL_H
constexpr int MAXTHREAD=4;
#include <pthread.h>
#include <unistd.h>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
typedef struct task__
{
public:
    task__(task_func& tf,void* data__):func(tf),data(data__)
    {}
    task__& operator=(const task__& other)=default;
    task_func func;
    void* data;
} Task;
class ThreadPool
{
public:
    typedef std::function<void(void*)> task_func;
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
    static void* Thread_run(void* arg);
    void run_task();
    Task Get_Task();
    std::queue<Task> taskqueue;
    bool isplaying;
    bool quit;
};

#endif // THREADPOOL_H
