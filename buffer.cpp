#include "buffer.hpp"
#include <cstring>
Buffer::Buffer(Buffer &&_buf):buf(std::move(_buf.getbuf())),maxvol(_buf.getMaxSize()),used(_buf.getusedsize())
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
int Buffer::Readfrombuf(char *rebuf, int &length)
{
    if(length<=used)
    {
        std::copy(buf.begin()+used-length,buf.begin()+used,rebuf);
        used-=length;
    }else{
        std::copy(buf.begin(),buf.begin()+used,rebuf);
        used=0;
        emptyCallback.func(emptyCallback.arg);
    }
    return length<used?length:used;
}
void Buffer::WriteFd(int fd, int &length)
{
    if(length<used)
    {
        auto it=buf.begin()+used-length;
        int n=write(fd,(void*)(&(*it)),length);
        length=n;
        used-=n;
    }
    else{
        auto it=buf.begin();
        int n=write(fd,(void*)(&(*it)),used);
        length=n;
        used-=n;
        if(used==0)
            emptyCallback.func(emptyCallback.arg);
    }
}
void Buffer::ReadFd(int fd, int &length)
{
    if(maxvol-used>=length)
    {
        auto it=buf.begin()+used;
        int n=read(fd,(void*)(&(*it)),length);
        length=n;
        used+=n;
    }else{
        int newlen=maxvol>length?2*maxvol:2*length;
        std::cout<<newlen;
       buf.reserve(newlen);
       maxvol=newlen;
       auto it=buf.begin()+used;
       int n=read(fd,(void*)(&(*it)),length);
       length=n;
       used+=n;
    }
}
int Buffer::Writeinbuf(char *content, int &length)
{
    if(length<=maxvol-used)
    {
        std::copy(content,content+length,buf.begin()+used);
        used+=length;
    }else{
        int _maxvol=maxvol>length?2*maxvol:2*length;
        buf.reserve(_maxvol);
        maxvol=_maxvol;
        auto it=buf.begin()+used;
        std::copy(content,content+length,it);
        used+=length;
        fullCallback.func(fullCallback.arg);
    }
    return length<=maxvol-used?length:maxvol-used;
}
