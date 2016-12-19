#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>


#include "net_server.h"
#include "net_session.h"

using namespace std;

static const int TIMEOUT = 5;

tcp_server::~tcp_server()
{
    stop();
}

tcp_server::tcp_server(const std::string &ip, uint32_t port):m_sockfd(-1)
  , m_ip(ip), m_port(port),m_stop(false),m_stopped(true)
{

}

void* tcp_server::listen_thread(void* p)
{
    tcp_server* obj = (tcp_server*)p;
#define MAX_CLINETS 1000
#define DELAY_MS 500
    //select
    fd_set readfds;
    fd_set writefds;

    struct timeval timeout;

    int sockfd = obj->sockfd();
    while (!obj->m_stop)
    {
        //清空读、写文件描述符集合
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        //计算最大文件描述符
        int max_fd = sockfd;
        //初始化读、写文件描述符集合
        FD_SET(sockfd, &readfds);

        for(list<client_sock>::iterator it= obj->m_clientfds.begin(); it!= obj->m_clientfds.end(); )
        {
            if(time(0) - it->start >= TIMEOUT)
            {
                it = obj->m_clientfds.erase(it);
                continue;
            }
            FD_SET(it->sockfd, &readfds);
            FD_SET(it->sockfd, &writefds);
            max_fd = max_fd > it->sockfd ? max_fd : it->sockfd;
            it++;
        }

        //构造超时结构体
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000 * DELAY_MS;

        int ret = select(max_fd + 1, &readfds, &writefds, 0, &timeout);
        if (ret == -1)
        {
            perror("select:");
            select(0, 0, 0,0, &timeout);
            continue;
        }
        //检查客户端socket是否可读（客户端是否发送消息）
        for(list<client_sock>::iterator it= obj->m_clientfds.begin(); it!= obj->m_clientfds.end();)
        {
            if (FD_ISSET(it->sockfd, &readfds))
            {
                if( obj->on_data_arriving(*it) != -1)
                {
                    it = obj->m_clientfds.erase(it);
                }
            }
            it++;
        }

        //检查是否有新的客户端连接
        if (FD_ISSET(sockfd, &readfds))
        {
            struct sockaddr_in cli_addr;
            socklen_t clilen = sizeof(cli_addr);
            int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
            if (newsockfd < 0)
            {
                perror("ERROR on accept");
            }

            client_sock cs;
            cs.sockfd = newsockfd;
            cs.start = time(0);
            obj->m_clientfds.push_back(cs);
        }

    }


    return 0;
}

void *tcp_server::session_thread(void *p)
{
    tcp_server* obj = (tcp_server*)p;

    timeval timeout;
    while (!obj->m_stop)
    {
        for(list<tcp_session*>::iterator it = obj->m_sessions.begin(); it!= obj->m_sessions.end();)
        {
            //如果session已经关闭，就删除该对象，并将其从监视列表清除
            if((*it)->closed())
            {
                delete *it;
                it = obj->m_sessions.erase(it);
                continue;
            }

            it++;
        }

        //构造超时结构体
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000 * DELAY_MS;

        select(0, 0, 0,0, &timeout);

    }

    return 0;
}

int tcp_server::start()
{
    if(!m_stopped)
    {
        return -1;
    }
    m_stopped = false;
    m_stop = false;

    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd < 0)
    {
        perror("ERROR opening socket");
        return errno;
    }

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;//todo m_ip
    serv_addr.sin_port = htons(m_port);
    if (bind(m_sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        return errno;
    }
    listen(m_sockfd,5);

    //确保工作线程都要创建成功，否则就不能继续运行
    if(pthread_create(&m_listenPID, 0, listen_thread, this) !=0)
    {
        m_stopped = true;
    }
    else
    {
        if(pthread_create(&m_sessionPID, 0, session_thread, this) !=0)
        {
            m_stop = true;

            pthread_join(m_listenPID, 0);
            m_stopped = true;
        }
    }


    return m_stopped?-1:0;
}

void tcp_server::stop()
{
    if(m_stopped)
    {
        return;
    }
    m_stop = true;
    //等待线程结束

    pthread_join(m_listenPID, 0);
    pthread_join(m_sessionPID, 0);


    for(list<tcp_session*>::iterator it = m_sessions.begin(); it!= m_sessions.end(); it++)
    {
        delete *it;
    }

    m_sessions.clear();
    for(list<client_sock>::iterator it = m_clientfds.begin(); it!= m_clientfds.end(); it++)
    {
        ::close(it->sockfd);
    }

    if(m_sockfd > 0)
    {
        ::close(m_sockfd);
    }

    m_stopped = true;
}

bool tcp_server::is_stopped() const
{
    return m_stopped;
}

int tcp_server::sockfd() const
{
    return m_sockfd;
}
