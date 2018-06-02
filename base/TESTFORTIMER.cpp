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
    t.Init(2,false);
    fuck.data.fd=(int)t.getfd();
    fuck.events|=EPOLLIN|EPOLLET;
    epoll_ctl(epfd,EPOLL_CTL_ADD,(int)t.getfd(),&fuck);
    std::function<void()> shit=callback;
    t.setCallback(shit);
    std::cout<<t.getTime()<<std::endl;
    int ret;
    if((ret=epoll_wait(epfd,&temp,1,0))<0)
        return 1;
    else
        t.handle();
    return 0;

}
