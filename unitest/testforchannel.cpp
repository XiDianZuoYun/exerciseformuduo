#include <unistd.h>
#include <iostream>
#include <sys/epoll.h>
#include <strings.h>
#include <functional>
#include "channel.h"
void readcallback()
{
     std::cout<<"fuck!"<<std::endl;
}
int main()
{

    Channel test(STDIN_FILENO,EPOLLET);
    test.enable_read();
    std::function<void()> temp=readcallback;
    test.setreadCallback(temp);
    int epfd=epoll_create(10);
    epoll_event tmp,ret;
    bzero(&tmp,sizeof(epoll_event));
    bzero(&ret,sizeof(epoll_event));
    tmp.data.fd=test.getfd();
    tmp.events=EPOLLET|EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,test.getfd(),&tmp);
    int n=epoll_wait(epfd,&ret,10,-1);
    if(n<0)
        return 1;
    if(n>0)
        test.setevents(ret.events);
    std::cout<<int(ret.events==EPOLLIN)<<std::endl;
    std::cout<<n<<std::endl;
    test.handle();
    return 0;
}
