#include "buffer.hpp"
#include <cstring>
Buffer::Buffer(Buffer &&_buf):buf(_buf.reset()),maxvol(_buf.getMaxSize()),used(_buf.getusedsize())
{
    _buf.~Buffer();
}
void Buffer::SetEmptyCallback(std::function<void *(void *)> &cb, void *arg)
{
    emptyCallback.func=functor(cb);
    emptyCallback.arg=arg;
}
void Buffer::SetFullCallback(std::function<void *(void *)> &cb, void *arg)
{
    emptyCallback.func=functor(cb);
    emptyCallback.arg=arg;
}
int Buffer::Readfrombuf(unsigned char *rebuf, int &length)
{
    MutexLockGuard __lock(mutex_);
    if(length<=used)
    {
        memcpy((void*)rebuf,(void*)buf,length);
        used-=length;
        buf-=length;
    }else{
        memcpy((void*)rebuf,(void*)buf,used);
        used=0;
        buf-=used;
        emptyCallback.func(emptyCallback.arg);
    }
    return length<used?length:used;
}
int Buffer::Writeinbuf(unsigned char *content, int &length)
{
    MutexLockGuard __lock(mutex_);
    if(length<=maxvol-used)
    {
        memcpy((void*)buf,(void*)content,length);
        used+=length;
        buf+=length;
    }else{
        int _maxvol=maxvol>length?2*maxvol:2*length;
        reusedbuf=(unsigned char*)malloc(_maxvol);
        maxvol=_maxvol;
        memcpy((void*)reusedbuf,(void*)buf,used);
        reusedbuf+=used;
        memcpy((void*)reusedbuf,(void*)content,length);
        used+=length;
        buf=reusedbuf+length;
        reusedbuf=nullptr;
        fullCallback.func(fullCallback.arg);
    }
    return length<=maxvol-used?length:maxvol-used;
}
unsigned char* Buffer::reset()
{
    MutexLockGuard __lock(mutex_);
    unsigned char* temp=buf;
    buf=nullptr;
    used=0;
    return temp;
}
