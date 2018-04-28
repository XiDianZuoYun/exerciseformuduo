#ifndef BUFFER_HPP
#define BUFFER_HPP
#include <MUTEXLOCK.hpp>
#include <functional>
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
    Buffer(int volumn):maxvol(volumn) {buf=(unsigned char*)malloc(maxvol);}
    Buffer(Buffer& _buf)=delete;
    Buffer(Buffer &&_buf);
    unsigned char* getBufferPtr(){return buf;}
    int Writeinbuf(unsigned char* content,int &length);
    int Readfrombuf(unsigned char* rebuf,int &length);
    int getusedsize(){return used;}
    int getMaxSize(){return maxvol;}
    void SetEmptyCallback(std::function<void*(void*)> &cb,void* arg);
    void SetFullCallback(std::function<void*(void*)> &cb,void* arg);
    unsigned char* reset();
    ~Buffer(){delete(buf);}
private:
    int maxvol;
    int used;
    unsigned char* buf;
    unsigned char* reusedbuf;
    MutexLock mutex_;
    CallBack emptyCallback;
    CallBack fullCallback;
};
#endif // BUFFER_HPP
