#ifndef __net_session_h__
#define __net_session_h__


#include <pthread.h>

class tcp_session
{
public:
    tcp_session(int sock);
    virtual ~tcp_session();
    int start();
    int close();
protected:
    int m_sockfd;
    pthread_t m_tid;
    virtual void handle_recv_data() = 0;
};


#endif
