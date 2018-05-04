#include<iostream>
#include "eventloop.hpp"
#include "Channel.hpp"
int main()
{
    EventLoop loop;
    Channel ch(STDIN_FILENO,EPOLLET|EPOLLIN,20,&loop);
    Channel* _ch=&ch;
    loop.updateChannel(_ch);
    loop.loop();
    loop.quit();
    return 0;
}
