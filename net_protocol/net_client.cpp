#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/select.h>

#include "net_client.h"

using namespace std;

tcp_client::tcp_client():m_connected(false),m_sockfd(-1),m_buffer(0),m_buffer_size(0)
{

}

tcp_client::~tcp_client()
{
    close();
    if(m_buffer !=0)
    {
        free(m_buffer);
        m_buffer = 0;
    }
}

int tcp_client::connect(const std::string &ip, uint16_t port)
{

    struct sockaddr_in serv_addr;

    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd < 0)
    {
        perror("ERROR opening socket");

        return -1;
    }
    struct hostent *server = gethostbyname(ip.c_str());
    if (server == 0)
    {
        fprintf(stderr,"ERROR, no such host\n");
        return -1;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(port);
    if (::connect(m_sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        return -1;
    }

    m_point.ip = ip;
    m_point.port = port;

    return 0;
}

int tcp_client::read(string &data, size_t len)
{
    if(m_buffer_size < len)
    {
        m_buffer = (uint8_t*)realloc(m_buffer, len);
        m_buffer_size = len;
    }

    ssize_t size = ::read(m_sockfd, m_buffer, len);

    if(size <= 0)
    {
        return size;
    }

    if(data.capacity() < data.length()+ size)
    {
        data.reserve(data.length()+size);
    }

    data.insert(data.length(), (const char*)m_buffer, size);

    return size;
}

int tcp_client::write(const string &data)
{
    return ::write(m_sockfd, data.data(), data.length());
}

void tcp_client::close()
{

}
