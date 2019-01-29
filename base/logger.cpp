#include "logger.h"
#define LOGSIZE 128
#define TOVOIDPTR(P) static_cast<void*>(P)
Logger* Logger::instance;
pthread_once_t Logger::init_;
using strvec=std::vector<std::string>;
Logger::Logger()
{

    logqueue[0]=std::make_shared<strvec>(0);
    logqueue[0]->reserve(LOGSIZE);
    logqueue[1]=std::make_shared<strvec>(0);
    pthread_spin_init(&splock,PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&mutex,nullptr);
    pthread_cond_init(&cond,NULL);
    if(filename.size()==0)
        filename=std::string("/home/zuoyun/Documents/");
    filename+=std::to_string(getpid());
    printf(filename.c_str());
    logfd=open(filename.c_str(),O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
    assert(logfd>0);
 //   logfd=STDOUT_FILENO;
    unsigned long per=1;
    pthread_create(&workthread,nullptr,Logger::Writewithctrl,&per);
    pthread_detach(workthread);
}
void Logger::Log(const std::string &logstr, level l)
{
    pthread_spin_lock(&splock);
    std::string temp=logstr;
    temp+=ToString(l);
    temp+="\r\n";
    if(logqueue[0]->size()==LOGSIZE)
        Channgebuf();
    logqueue[0]->push_back(temp);
    pthread_spin_unlock(&splock);
}
void* Logger::WriteInfile(void* permanent)
{
    unsigned long flag=(unsigned long)permanent;
    //I don't know why, there is always a race condition here, I can only do it with sleep.
    sleep(1);
    pthread_mutex_lock(&mutex);
    do
    {
        while(logqueue[1]->empty())
        {
            pthread_cond_wait(&cond,&mutex);
        }
        queueptr writer(logqueue[1]);
        logqueue[1]=std::make_shared<strvec>(0);
        while (writer->empty()==false) {
            std::string& temp=writer->back();
            int val=write(logfd,TOVOIDPTR(const_cast<char*>(temp.c_str())),temp.size());
            assert(val==temp.size());
#ifdef DEBUG
            printf(temp.c_str());
#endif
            writer->pop_back();
        }
        assert(fsync(logfd)==0);
#ifdef DEBUG
        printf("All data has been writen!");
#endif
        pthread_mutex_unlock(&mutex);
    }while(flag!=0);
    return (void*)0;
}
std::string Logger::GetTime()
{
    timeval tv;
    gettimeofday(&tv,nullptr);
    tm *t=localtime(&(tv.tv_sec));
    std::stringstream ss;
    ss<<t->tm_year<<t->tm_mon<<t->tm_mday<<t->tm_hour<<t->tm_min;
    return ss.str();
}
void Logger::Channgebuf()
{
    using strvec=std::vector<std::string>;
    pthread_spin_lock(&splock);
    if(logqueue[1]->size()!=0)
        logqueue[1]=std::make_shared<strvec>(1);
    std::swap(logqueue[0],logqueue[1]);
    pthread_spin_unlock(&splock);
    pthread_cond_signal(&cond);
}
void Logger::Make_workthread()
{
    pthread_t t;
    workqueue.push_back(t);
    unsigned long per=0;
    pthread_create(&t,nullptr,Logger::Writewithctrl,static_cast<void*>(&per));
    pthread_detach(t);
}

