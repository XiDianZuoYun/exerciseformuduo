#ifndef BUFFER_H
#define BUFFER_H
#include <vector>
#include <mutex>
#include <assert.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>
class Buffer
{
public:
    Buffer();
    char* getdata(int32_t length);
    char* takedata(int32_t length);
    void wirtein(const char* src,int32_t length);
    int32_t readfd(int fd);
    int32_t writefd(int fd,int32_t length);
private:
    int32_t maxsize;
    int32_t readindex;
    int32_t readbytes;
    int32_t writeindex;
    std::mutex mutex_;
    std::vector<char> buffer_;
};

#endif // BUFFER_H
