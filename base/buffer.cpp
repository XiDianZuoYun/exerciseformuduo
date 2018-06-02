#include "buffer.h"

Buffer::Buffer():buffer_(1024,'\0'),maxsize(1024),readindex(0),readbytes(0),
  writeindex(0),mutex_()
{
    assert(buffer_.capacity()==1024);
}
char* Buffer::getdata(int32_t length)
{
    assert(length>0);
    //std::unique_lock<std::mutex> lockguard(mutex_);
    char* temp=buffer_.data();
    if(length>readbytes)
    {
        char* ret=(char*)malloc(readbytes*sizeof(char));
        memcpy((void*)ret,(void*)(temp+readindex),readbytes);
    }else{
        char* ret=(char*)malloc(length*sizeof(char));
        memcpy((void*)ret,(void*)(temp+readindex),length);
        if(readindex>maxsize/2)
        {
            memmove((void*)(temp),(void*)(temp+readindex),readbytes);
            memset((void*)(temp+readindex),'\0',readbytes);
            writeindex=readindex+readbytes;
        }
    }
    return temp;
}
char* Buffer::takedata(int32_t length)
{
    char* ret=getdata(length);
    readindex=readindex+(length>readbytes?readbytes:length);
}
void Buffer::wirtein(const char *src, int32_t length)
{
    assert(length>=0);
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
    memmove((void*)temp,(void*)src,length);
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
int32_t Buffer::writefd(int fd, int32_t length)
{
    //std::unique_lock<std::mutex> lockguard(mutex_);
    char* data=buffer_.data()+readindex;
    int32_t n=0;
    if(readbytes<length)
        n=write(fd,(void*)data,readbytes);
    else
        n=write(fd,(void*)data,length);
    assert(n>=0);
    return n;
}
