#include "logger.hpp"
void logger::writelog()
{
    while(!quit)
    {
       _lock.lock();
       while(logqueue.empty())
            _cont.wait();
       std::string temp=logqueue.front();
       logqueue.pop();
       int len=temp.size();
       int n=fwrite(temp.c_str(),sizeof('a'),len,m_file);
       int sum=n;
       while (sum<len) {
           n=fwrite(temp.c_str()+n,sizeof('a'),len-n,m_file);
           assert(n>=0);
           sum+=n;
       }
    }
}
logger::logger(const std::string& path):m_file(open(path.c_str(),"a+")),
    _lock(),_cont(_lock),_quit(false)
{
    std::function<void()> func=std::bind(&logger::writelog,this);
    backgroud(func);
    backgroud.start();
}
