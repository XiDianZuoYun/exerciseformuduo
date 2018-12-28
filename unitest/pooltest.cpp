#include <iostream>
#include <vector>
#include <threadpool.h>
#include <unistd.h>
void test_func(void* a)
{
    int t=*((int*)a);
    t=t*t+2;
    std::cout<<t<<" ";
}
int main()
{
    std::function<void (void*)> func=&test_func;
    ThreadPool pool;
    pool.Start();
    int pa[1000]={0};
    for(int i=0;i<1000;++i){
        pa[i]=i;
        pool.push_task(func,(void*)(pa+i));
    }
    usleep(1000);
    pool.Quit();
    return 0;
}
