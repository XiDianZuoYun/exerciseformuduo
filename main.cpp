#include<iostream>
#include<fcntl.h>
#include "eventloop.hpp"
#include "Channel.hpp"
int main()
{
    EventLoop loop;

    Channel* _ch=new Channel(STDIN_FILENO,EPOLLIN,100,&loop);
    loop.updateChannel(_ch);
    loop.loop();
    loop.quit();
    return 0;
}
