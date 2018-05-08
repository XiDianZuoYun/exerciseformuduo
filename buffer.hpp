#ifndef BUFFER_HPP
#define BUFFER_HPP
#include <functional>
#include <vector>
#include "MUTEXLOCK.hpp"
class Buffer
{
public:
    typedef std::function<void*(void*)> functor;
    typedef struct CallBack
    {
        functor func;
        void* arg;
        CallBack() {}
    } CallBack;
    Buffer(int volumn):maxvol(volumn),buf(volumn,0){}
    Buffer(Buffer& _buf)=delete;
    Buffer(Buffer &&_buf);
    char* getBufferPtr(){return &(*buf.begin());}
    std::vector<char>& getbuf(){return buf;}
    int Writeinbuf(char* content,int &length);
    void WriteFd(int fd,int&length);
    int Readfrombuf(char* rebuf,int &length);
    void ReadFd(int fd,int& length);
    int getusedsize(){return used;}
    int getMaxSize(){return maxvol;}
    void SetEmptyCallback(std::function<void*(void*)> &cb,void* arg);
    void SetFullCallback(std::function<void*(void*)> &cb,void* arg);
    unsigned char* reset();
    ~Buffer(){}
private:
    int maxvol;
    int used;
    std::vector<char> buf;
    MutexLock mutex_;
    CallBack emptyCallback;
    CallBack fullCallback;
};
#endif // BUFFER_HPP
