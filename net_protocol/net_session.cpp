#include <unistd.h>

#include "net_session.h"
tcp_session::tcp_session(int sock)
{
    m_sockfd = sock;
}

tcp_session::~tcp_session()
{
    close();
}

int tcp_session::start()
{
    return 0;
}

int tcp_session::close()
{
    ::close(m_sockfd);
}
