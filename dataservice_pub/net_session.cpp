#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>


#include "net_session.h"
tcp_session::tcp_session(int sock):m_stop(false), m_stopped(true)
{
    m_sockfd = sock;
}

tcp_session::~tcp_session()
{
    close();
}

int tcp_session::open()
{
    if(!m_stopped)
    {
        return -1;
    }
    m_stop = false;
    if(pthread_create(&m_tid, 0, sock_thread, this) !=0)
    {
        m_stop = true;
    }

    return 0;
}

int tcp_session::close()
{
    m_stop = true;
    pthread_join(m_tid, 0);
    ::close(m_sockfd);
    m_sockfd = -1;
}

bool tcp_session::closed() const
{
    return m_stopped;
}


void *tcp_session::sock_thread(void *p)
{
    tcp_session* obj = (tcp_session*)p;
#define MAX_CLINETS 1000
#define DELAY_MS 500
    //select
    fd_set readfds;
    fd_set writefds;

    struct timeval timeout;
    obj->m_stopped = false;

    while(!obj->m_stop)
    {
        //清空读、写文件描述符集合
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        //计算最大文件描述符
        int max_fd = obj->m_sockfd;
        //初始化读、写文件描述符集合
        FD_SET(obj->m_sockfd, &readfds);
        FD_SET(obj->m_sockfd, &writefds);
        //构造超时结构体
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000 * DELAY_MS;

        int ret = select(max_fd + 1, &readfds, &writefds, 0, &timeout);
        if (ret == -1)
        {
            perror("select:");
            timeout.tv_usec = 1000 * DELAY_MS;
            select(0, 0, 0, 0, &timeout);
            continue;
        }
        if(ret == 0)
        {
            continue;
        }

        if(obj->handle_recv_data() == tcp_session::STOP)
        {
            break;
        }

    }

    obj->m_stopped = true;

    return 0;
}
