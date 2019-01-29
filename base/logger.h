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
     //Manage character queues with smart pointers
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
    //Log level, now only as a tag
    enum level{
        Trace,Debug,Warning,Error,Fatal
    } ;
    //uncopyable
    Logger(const Logger& other)=delete;
    Logger& operator=(const Logger &other)=delete;
    ~Logger()=default;
    //Refer to the implementation of muduo, use pthread_once_t to prevent race condition
    static Logger* Getinstance()
    {
        pthread_once(&Logger::init_,Logger::init);
        return Logger::instance;
    }
    //User interface, putting user-defined strings and levels into the queue
    void Log(const std::string& logstr,Logger::level l);
    std::string ToString(level l)
    {
        return LEVEL[int(l)];
    }
    //Set the name of log file
    void SetFileName(const std::string& name)
    {
        filename=name;
    }
    //Use gilbc's function gettimeofday() to get the current time.
     std::string GetTime();
    //Refresh existing log content and write it to file
    void Flush()
    {
        Channgebuf();
    }
private:
    //Write the log to the file, executed by the child thread
    void* WriteInfile(void* permanent);
    static void* Writewithctrl(void* permanent)
    {
        Logger::instance->WriteInfile(permanent);
    }
};

#endif // LOGGER_H
