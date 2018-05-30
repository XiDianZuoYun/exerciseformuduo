#include <iostream>
#include <sys/epoll.h>
#include <timer.h>
#include <string.h>
void callback()
{
    std::cout<<"go!"<<std::endl;
}
int main()
{
    timer t;
    int epfd=epoll_create(10);
    epoll_event fuck,temp;
    t.Init(4,false);
    memset(&fuck,0,sizeof(fuck));
    fuck.data.fd=(int)t.getfd();
    fuck.events|=EPOLLIN|EPOLLET;
    assert(epoll_ctl(epfd,EPOLL_CTL_ADD,t.getfd(),&fuck)==0);
    std::function<void()> shit=callback;
    t.setCallback(shit);
    std::cout<<t.getTime()<<std::endl;
    int ret;
    if((ret=epoll_wait(epfd,&temp,1,-1))<0)
        return 1;
    else
    {
        t.handle();
        std::cout<<int(temp.data.fd==t.getfd())<<" "<<temp.events;
        return 0;
    }

}
