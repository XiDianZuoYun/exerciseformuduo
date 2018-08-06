#include "socket.h"
#include <iostream>
#include <QDebug>
using namespace std;
int main()
{
    Socket testsock("TCP");
    std::cout<<"build!";
//    testsock.Block(true);
    testsock.Bind(9999);
    testsock.Listen(20);
    qDebug() << "begin to listen";
    std::cout<<"begin to listen";
    int new_sock=0;
    new_sock=testsock.Accept();
    char* buf=(char*)malloc(10);
    send(new_sock,(void*)buf,5,SOCK_NONBLOCK);
    return 0;
}
