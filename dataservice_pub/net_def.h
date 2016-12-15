#ifndef __net_def_h__
#define __net_def_h__

#include <string>
#include <stdint.h>
using std::string;

struct NetPoint
{
    string ip;
    uint16_t port;
};

struct client_sock
{
    int sockfd;
    std::string  data;
    time_t start;
};



#endif
