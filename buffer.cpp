#include "buffer.hpp"
#include <cstring>
Buffer::Buffer(Buffer &&_buf):buf(_buf.reset()),maxvol(_buf.getMaxSize()),used(_buf.getusedsize())
{
    _buf.~Buffer();
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
        memcpy((void*)buf,(void*)content,length);
        used=maxvol;
        buf+=maxvol-used;
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
