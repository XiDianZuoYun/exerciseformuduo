#ifndef UDPSERVER_H
#define UDPSERVER_H
#include <unordered_map>
#include <string>
#include <queue>
#include <functional>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "poller.h"
#include "socket.h"
#include "eventloop.h"
#include "buffer.h"
typedef struct UdpBuffer
{
    Buffer* rbuf;
    Buffer* wbuf;
    void* Private_Data=nullptr;
    std::function<void(void*)> Destroy;
    ~UdpBuffer()
    {
        delete rbuf;
        delete wbuf;
        Destroy(Private_Data);
    }
} UdpBuffer;
class UdpServer
{
public:
    typedef std::function<void(const std::string& ip,uint16_t port,UdpBuffer* buf)> Callback;
    typedef std::function<void(const std::string& ip,uint16_t port,char* buf,size_t buf_len)> Temp_Callback;
    UdpServer(int maxevents=1024);
    ~UdpServer() noexcept;
    void bind(uint16_t port);
    int32_t Send_To(std::string& ip,uint16_t port,char* buff,int32_t buflen);
    void Remove_Udp_Buffer(std::string& ip,uint16_t port)
    {
        uint64_t index=ToIndex(ip,port);
        UdpBuffer* UB=data_map[index];
        data_map.erase(index);
        delete  UB;
    }
    void run();
    void Stop();
    //To Do
private:
    //To Do

    uint64_t ToIndex(const char* ip,uint16_t port)
    {
        uint64_t ret=0;
        uint32_t addr=inet_addr(ip);
        assert(addr!=0);
        ret|=addr;
        ret=ret<<32;
        ret|=port;
        return ret;
    }
    uint64_t ToIndex(std::string& ip,uint16_t port)
    {
        return ToIndex(ip.c_str(),port);
    }
    std::queue<uint64_t> Reg_Write;
    void Udp_Accept();
    void Udp_Write();
    int max_buf_nums;
    int nums_on_work;
    std::unordered_map<uint64_t,UdpBuffer*> data_map;
    EventLoop* __loop;
    uint16_t port;
    Socket* udp_sock;
    Channel* __chan;
    Temp_Callback Tmp_CallBack;
    Callback Message_CallBack;
};

#endif // UDPSERVER_H
