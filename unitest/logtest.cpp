#include "base/logger.h"
using namespace std;
#ifndef DEBUG
#define DEBUG
#endif
int main()
{
    Logger* log=Logger::Getinstance();
//    log->SetFileName("text");
    for(int i=0;i<10;++i)
        log->Log("fuck\n",Logger::Debug);
    log->Flush();
    sleep(20);
    return 0;
}
