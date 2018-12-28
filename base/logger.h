#ifndef LOGGER_H
#define LOGGER_H
#include <pthread.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include <vector>
#include <string>
#include <sstream>
#include <memory>

class Logger
{
private:
     typedef std::shared_ptr<std::vector<std::string>> queueptr;
     std::string filename;
     static Logger* instance;
     static pthread_once_t init_;
     pthread_t workthread;
     pthread_spinlock_t splock;
     pthread_mutex_t mutex;
     pthread_cond_t cond;
     std::vector<pthread_t> workqueue;
     const std::string LEVEL[5]={"Trace","Debug","Warning","Error","Fatal"};
     int logfd;
     static void init()
     {
         Logger::instance=new Logger();
     }
     queueptr logqueue[2];
     void Make_workthread();
     void Channgebuf();
     Logger();
public:
    enum level{
        Trace,Debug,Warning,Error,Fatal
    } ;
    //uncopyable
    Logger(const Logger& other)=delete;
    Logger& operator=(const Logger &other)=delete;
    ~Logger()=default;
    static Logger* Getinstance()
    {
        pthread_once(&Logger::init_,Logger::init);
        return Logger::instance;
    }
    void Log(const std::string& logstr,Logger::level l);
    std::string ToString(level l)
    {
        return LEVEL[int(l)];
    }
    void SetFileName(const std::string& name)
    {
        filename=name;
    }
     std::string GetTime();
    void Flush()
    {
        Channgebuf();
    }
    void* WriteInfile(void* permanent);
    static void* Writewithctrl(void* permanent)
    {
        Logger::instance->WriteInfile(permanent);
    }
};

#endif // LOGGER_H
