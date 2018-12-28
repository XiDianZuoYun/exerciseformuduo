#include "acceptor.h"
#include "poller.h"
#include <iostream>
#include <vector>
#define DEBUG
int main()
{
    Acceptor ac(nullptr);
    ac.Bind(8888);
    ac.Listen(100);
    Poller po(100);
    po.update_channel(ac.getChannel());
    std::vector <Channel*> __test;
    po.poll(__test);
    std::cout<<__test.size()<<std::endl;
    std::cout<<"pause";
    std::cout<<EPOLLERR<<" "<<EPOLLIN<<" "<<EPOLLHUP<<" "<<EPOLLOUT ;
}
