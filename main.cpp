#include "threadpool.hpp"
#include <functional>
using namespace std;

#include <pthread.h>
void* testthreadpool(void* v)
{
    std::cout<<(long)v<<" "<<(int)pthread_self();
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
    return 0;
}
