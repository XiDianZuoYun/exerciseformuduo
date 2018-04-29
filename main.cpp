#include <pthread.h>
#include <functional>
#include<QDebug>
#include "CONDITION.hpp"
#include "threadpool.hpp"
using namespace std;
void* testthreadpool(void* v)
{
    std::cout<<(long)v<<std::endl;
    return (void*)0;
}
int main()
{
    ThreadPool pool;
    std::function<void* (void*)> func=testthreadpool;
    for(long i=0;i<100;++i)
    {
        ThreadPool::task temp;
        temp.arg=(void*)i;
        temp.func=func;
        pool.addtask(temp);
    }
    pool.startpool();
    sleep(3);
    pool.quit();
    return 0;
}
