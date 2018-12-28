#ifndef BUFFER_H
#define BUFFER_H
#include <vector>
#include <mutex>
#include <utility>
#include <assert.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>
#include <jemalloc/jemalloc.h>
#include "commonexception.h"
#define TOVOIDPTR(p) static_cast<void*>(p)
#define TOCHARPTR(p) static_cast<char*>(p)
class Buffer
{
public:
    //"get" means copy,not move.Data will remain in the buffer after action.
    //"take" means move.
    Buffer(size_t bufsize=1024);
    ~Buffer()=default;
    char* getdata(int32_t length);
    char* takedata(int32_t length);
    int getdata(char* buf,int32_t length);
    int takedata(char* buf,int32_t length);
    std::pair<const char*,int32_t> readdata();
    void wirtein(const char* src,int32_t length);
    int32_t readfd(int fd);
    int32_t writefd(int fd,int32_t length);
    int32_t getSize(){return readbytes;}
private:
    void UpdateIndex()
    {
        char* temp=buffer_.data();
        if(readindex>maxsize/2)
        {
            memmove(TOVOIDPTR(temp),TOVOIDPTR(temp+readindex),readbytes);
            memset(TOVOIDPTR(temp+readindex),'\0',readbytes);
            readindex=0;
            writeindex=readindex+readbytes;
        }
    }
    int32_t maxsize;
    int32_t readindex;
    int32_t readbytes;
    int32_t writeindex;
    std::mutex mutex_;
    std::vector<char> buffer_;
};

#endif // BUFFER_H
