#include <iostream>
#include <sys/epoll.h>
#include <timer.h>
void callback()
{
    std::cout<<"go!"<<std::endl;
}
int main()
{
    timer t;
    int epfd=epoll_create(10);
    epoll_event fuck,temp;
    fuck.data.fd=(int)t.getfd();
    fuck.events|=EPOLLIN|EPOLLET;
    epoll_ctl(epfd,EPOLL_CTL_ADD,(int)t.getfd(),&fuck);
    std::function<void()> shit=callback;
    t.setCallback(shit);
    t.Init(2.2,false);
    int ret;
    if((ret=epoll_wait(epfd,&temp,1,0))<0)
        return 1;
    else
        t.handle();
    return 0;

}
