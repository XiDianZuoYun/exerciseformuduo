#include "udpserver.h"
#include "channel.h"
UdpServer::UdpServer(int maxevents):max_buf_nums(maxevents),nums_on_work(0),
    __loop(new EventLoop(maxevents,this)),
    udp_sock(new Socket("UDP"))
{
    __chan=new Channel(udp_sock->getfd(),EPOLLIN|EPOLLET,__loop);
    std::function<void()> rfunc=std::bind(&UdpServer::Udp_Accept,this);
    std::function<void()> wfunc=std::bind(&UdpServer::Udp_Write,this);
    __chan->setreadCallback(rfunc);
    __chan->setWriteCallback(wfunc);
    __loop->updateChannel(__chan);
}
UdpServer::~UdpServer() noexcept
{
    __loop->stop();
    delete __loop;
    delete udp_sock;
    for(auto &p:data_map)
    {
        delete p.second;
    }
}
void UdpServer::bind(uint16_t port)
{
    udp_sock->Bind(port);

}
void UdpServer::run()
{
    udp_sock->Listen(max_buf_nums/10);
    __loop->loop();
}
int32_t UdpServer::Send_To(std::string &ip, uint16_t port, char *buff, int32_t buflen)
{
    int32_t size=udp_sock->Sendto(buff,buflen,ip.c_str(),port);
    if(size==buflen)
        return size;
    uint64_t index=ToIndex(ip,port);
    if(data_map.count(index)!=0)
    {
        data_map[index]->wbuf->setinbuf(buff+size,buflen-size);
    }else if (nums_on_work<max_buf_nums) {
        data_map[index]=new UdpBuffer();
        data_map[index]->rbuf=new Buffer();
        data_map[index]->wbuf=new Buffer();
        __chan->enable_write();
    }
}
void UdpServer::Udp_Accept()
{
    char ip[16]={'\0'};
    size_t buflen=65535;
    char buff[65535]={'\0'};
    uint16_t __port=0;
    while (udp_sock->Recvfrom(buff,buflen,ip,__port)>0) {
        uint64_t index=ToIndex(ip,__port);
        if(data_map.count(index)!=0)
        {
            //To Do
            data_map[index]->rbuf->setinbuf(buff,int32_t(buflen));
        }else {
            if(max_buf_nums==nums_on_work)
            {
                if(Tmp_CallBack!=nullptr)
                    Tmp_CallBack(ip,__port,buff,buflen);
                continue;
            }else {
                ++nums_on_work;
                data_map[index]=new UdpBuffer ();
                data_map[index]->rbuf=new Buffer();
                data_map[index]->wbuf=new Buffer();
            }
        }
        if(Message_CallBack!=nullptr)
        {
            Message_CallBack(ip,__port,data_map[index]);
        }
        memset(static_cast<void*>(ip),'\0',16);
        memset(static_cast<void*>(buff),'\0',buflen);
    }
}
void UdpServer::Udp_Write()
{
    while(!Reg_Write.empty())
    {
        uint64_t index=Reg_Write.front();
        Buffer* buf=data_map[index]->wbuf;
        int32_t origin_length=buf->getSize();
        int32_t length=buf->writefd(udp_sock->getfd(),buf->getSize());
        if(origin_length==length)
            Reg_Write.pop();
    }
}
