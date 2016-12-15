#ifndef __net_session_h__
#define __net_session_h__


#include <pthread.h>
#include <string>

class tcp_session
{
public:
    tcp_session(int sock);
    virtual ~tcp_session();
    int open();
    int close();
protected:
    bool m_stop;
    int m_sockfd;
    std::string  m_data;
    pthread_t m_tid;
    static void* sock_thread(void *);
    virtual int  handle_recv_data() = 0;
};


#endif
