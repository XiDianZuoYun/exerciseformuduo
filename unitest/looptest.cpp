#include <iostream>
#include "base/eventloop.h"
#define DEBUG
using namespace std;
void call_test()
{
    std::cout<<"Hello World"<<std::endl;
}
int main()
{
    std::function<void()> func=std::bind(&call_test);
    EventLoop l(100);
    l.Bind_Port(8888);
    l.runAfter(func,100);
    l.loop();
    return 0;
}
