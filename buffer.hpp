#ifndef BUFFER_HPP
#define BUFFER_HPP
#include <MUTEXLOCK.hpp>
#include <functional>
class Buffer
{
public:
    Buffer(int volumn):maxvol(volumn) {}
    Buffer(Buffer& _buf)=delete;
    Buffer(Buffer &&_buf);
    unsigned char* getBufferPtr(){return buf;}
    int Writeinbuf(unsigned char* content,int &length);
    int Readfrombuf(unsigned char* rebuf,int &length);
    int getusedsize(){return used;}
    int getMaxSize(){return maxvol;}
    void SetEmptyCallback(std::function<void*(void*)> &cb){emptyCallback=cb;}
    void SetFullCallback(std::function<void*(void*)> &cb){fullCallback=cb;}
    unsigned char* reset();
    ~Buffer(){delete(buf);}
private:
    int maxvol;
    int used;
    unsigned char* buf;
    MutexLock mutex_;
    std::function<void* (void*)> emptyCallback;
    std::function<void* (void*)> fullCallback;
};
#endif // BUFFER_HPP
