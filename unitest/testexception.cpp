#include <iostream>
#include "base/commonexception.h"
using namespace std;
int main()
{
    int i=0;
    try{
        CHECK_COND(i==1,"fuck",CommonException::BUFFERErr);
    }catch(CommonException& c)
    {
        std::cout<<c.what();
    }
    return 0;
}
