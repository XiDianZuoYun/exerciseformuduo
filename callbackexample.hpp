#ifndef CALLBACKEXAMPLE_HPP
#define CALLBACKEXAMPLE_HPP
#include <functional>
#include <vector>
#include <sys/socket.h>
#include "buffer.hpp"
#include "Channel.hpp"
class TemplateCallback
{
public:
    typedef std::function<void*(void*)> functor;
    functor readCallback;
    functor writeCallback;
    functor ConnectionCallback;
private:
    static void* _read(void*);
    static void* _connect(void*);
    static void* _write(void*);
};
#endif // CALLBACKEXAMPLE_HPP
