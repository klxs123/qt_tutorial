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
tcp_server::~tcp_server()
{
    for(list<tcp_session*>::iterator it = m_sessions.begin(); it!= m_sessions.end(); it++)
    {
        delete *it;
    }

    m_sessions.clear();
}

tcp_server::tcp_server(const std::string &ip, uint32_t port):m_sockfd(-1)
  , m_ip(ip), m_port(port),m_stop(false)
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

    char buffer[256];

    struct timeval timeout;

    int sockfd = obj->sockfd();
    while (!obj->is_stop())
    {
        //清空读、写文件描述符集合
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        //计算最大文件描述符
        int max_fd = sockfd;
        //初始化读、写文件描述符集合
        FD_SET(sockfd, &readfds);

        for(list<client_sock>::const_iterator it= obj->m_clientfds.begin(); it!= obj->m_clientfds.end(); it++)
        {
           FD_SET(it->sockfd, &readfds);
           FD_SET(it->sockfd, &writefds);
           max_fd = max_fd > it->sockfd ? max_fd : it->sockfd;
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
        for(list<client_sock>::iterator it= obj->m_clientfds.begin(); it!= obj->m_clientfds.end(); it++)
        {
            if (FD_ISSET(it->sockfd, &readfds))
            {
                obj->on_data_arriving(*it);
            }

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
            obj->m_clientfds.push_back(cs);
        }

    }

    return 0;
}

int tcp_server::start()
{
    stop(true);
    if(m_sockfd > 0 )
    {
        ::close(m_sockfd);
    }

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
    if(pthread_create(&m_listenPID, 0, listen_thread, this) ==0)
    {
        m_stop = false;
    }
    else
    {
        m_stop = true;
    }

    return m_stop?-1:0;
}

void tcp_server::stop(bool sync)
{
    m_stop = true;

    if(sync)
    {
        pthread_join(m_listenPID, 0);
    }
}

bool tcp_server::is_stop() const
{
    return m_stop;
}

int tcp_server::sockfd() const
{
    return m_sockfd;
}
