#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "base/poller.h"
using namespace std;
int main()
{
    Poller poll_test;
    int sock=socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in temp;
    bzero(&temp,sizeof(sockaddr_in));
    temp.sin_family=AF_INET;
    temp.sin_port=htons(8888);
    temp.sin_addr.s_addr=htonl(INADDR_ANY);
    bind(sock,(sockaddr*)&temp,sizeof(sockaddr_in));
    poll_test.AddFD(sock);
    poll_test.Poll_debug();
    return 0;
}
