#ifndef __net_server_h__
#define __net_server_h__

#include <string>
#include <stdint.h>
#include <list>
#include <pthread.h>

#include "net_def.h"


class tcp_session;

class tcp_server
{
public:
    tcp_server(const std::string& ip, uint32_t port );
    virtual ~tcp_server();
    int start();
    void stop(bool sync=false);
    bool is_stop() const;
    int sockfd() const;
protected:
    std::string m_ip;
    uint32_t m_port;
    std::list<tcp_session*> m_sessions;
    std::list<client_sock> m_clientfds;
    int m_sockfd;
    pthread_t m_listenPID;
    bool m_stop;
    static void* listen_thread(void* p);

    virtual int  on_data_arriving(client_sock&) = 0;
};


#endif
