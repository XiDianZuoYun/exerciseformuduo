#include "buffer.h"
#define DEBUG
#define CHECK(length) if(length<=0) \
    throw CommonException("Invalid length!",CommonException.ErrType::BUFFERErr);
Buffer::Buffer(size_t bufsize):buffer_(bufsize,'\0'),maxsize(1024),readindex(0),readbytes(0),
  writeindex(0),mutex_()
{
    assert(buffer_.capacity()==1024);
}
//copy,not move
char* Buffer::getdata(int32_t length)
{
#ifdef DEBUG
    assert(length>0);
#else
    CHECK(length);
#endif
    //std::unique_lock<std::mutex> lockguard(mutex_);
    char* temp=buffer_.data();
    char* ret=TOCHARPTR(malloc(length*sizeof(char)));
    if(length>readbytes)
    {
        memcpy(TOVOIDPTR(ret),TOVOIDPTR(temp+readindex),readbytes);
    }else{
       memcpy((void*)ret,(void*)(temp+readindex),length);
    }
    return ret;
}
//copy,not move
int32_t Buffer::getdata(char* buf,int32_t length)
{
    int32_t len=length<readbytes?length:readbytes;
    memcpy(TOVOIDPTR(buf),TOVOIDPTR(buffer_.data()),len);
    return len;
}
//move,not copy
char* Buffer::takedata(int32_t length)
{
    char* ret=getdata(length);
    readindex=readindex+(length>readbytes?readbytes:length);
    readbytes=readbytes-length>0?readbytes-length:0;
    UpdateIndex();
    return ret;
}
int32_t Buffer::takedata(char* buf,int32_t length)
{
#ifdef DEBUG
    assert(length>0);
#else
    CHECK(length);
#endif
    int len=length<readbytes?length:readbytes;
    memcpy(TOVOIDPTR(buf),TOVOIDPTR(buffer_.data()),len);
    readindex=readindex+len;
    readbytes=readbytes-len;
    UpdateIndex();
    return len;
}
void Buffer::wirtein(const char *src, int32_t length)
{
#ifdef DEBUG
    assert(length>0);
#else
    CHECK(length);
#endif
    //std::unique_lock<std::mutex> lockguard(mutex_);
    if(length==0)
        return;
    //user should make sure that they do the volumn of src is enough
    int32_t writeable=maxsize-writeindex;
    if(writeable<length){
        int32_t newsize=length>maxsize?2*length:2*maxsize;
        buffer_.resize(newsize,'\0');
    }
    char* temp=buffer_.data()+maxsize-writeable;
    memmove(temp,src,length);
    writeindex+=length;
    readbytes+=length;
}
int32_t Buffer::readfd(int fd)
{
    //like muduo::buffer,use readv() to avoid twice sys_call
    //std::unique<std::mutex> lockguard(mutex_);
    char extrabuf[65535];
    struct iovec vec[2];
    int32_t writeable=maxsize-writeindex;
    vec[0].iov_base=(void*)(buffer_.data()+writeindex);
    vec[0].iov_len=writeable;
    vec[1].iov_base=(void*)extrabuf;
    vec[1].iov_len=65535;
    const int32_t n=readv(fd,vec,2);
    assert(n>=0);
    if(n>writeable)
        this->wirtein(extrabuf,n-writeable);
    else
    {
        writeindex+=n;
        readbytes+=n;
    }
    return n;
}
int32_t Buffer::setinbuf(char *buf, int32_t buflen)
{
    if(maxsize-writeindex<=buflen)
        Expand(maxsize-writeindex);
    buffer_.insert(buffer_.end(),buf,buf+buflen);
    return buflen;
}
int32_t Buffer::writefd(int fd, int32_t length)
{
    //std::unique_lock<std::mutex> lockguard(mutex_);
    char* data=buffer_.data()+readindex;
    int32_t n=0;
    if(readbytes<length)
        n=write(fd,(void*)data,readbytes);
    else
        n=write(fd,(void*)data,length);
#ifdef DEBUG
    assert(n>=0);
#else
    CHECK_COND(n>=0,"writefd has a failure",CommonException.FILEEXCErr);
#endif
    readindex+=n;
    readbytes-=n;
    UpdateIndex();
    return n;
}
std::pair<const char*,int32_t> Buffer::readdata()
{
    const char* temp=buffer_.data()+readindex;
    return std::pair<const char*,int32_t>(temp,readbytes);
}
