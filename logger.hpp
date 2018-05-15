#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <fcntl.h>
#include <stdio.h>
#include <queue>
#include <string>
#include "MUTEXLOCK.hpp"
#include "THREAD.h"
#include "CONDITION.hpp"
#ifdef LOGGER_HPP
class logger{
public:
    log(const std::string& path);
    ~log()
    {
        logqueue.swap(std::queue<std::string>());
    }
    void AddLog(const char* pszinfo,const char*filename,int lineNO,
                const char* pszFuncSig,const char* args);
private:
    bool _quit;
    void writelog();
    MutexLock _lock;
    condition _cont;
    std::queue<std::string> logqueue;
    Thread backgroud;
    FILE* m_file;
}
#endif // LOGGER_HPP
