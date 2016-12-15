#include <unistd.h>
#include "zf_net_session.h"
#include "protocol_parser.h"
#include "protocol_utils.h"

zf_tcp_session::zf_tcp_session(int sock):tcp_session(sock)
{

}

int  zf_tcp_session::handle_recv_data()
{
    char buf[1024] ={0};
    ssize_t size = 0;
    do
    {
        size = ::read(m_sockfd, buf, 1024);
        if(size > 0)
        {
            m_data.append(buf, size);
        }

    }while(size > 0 && size == 1024);

    string pkg;
    extract_packages(m_data, pkg);

    Message msg;
    package_to_msg(msg, pkg);

    switch(msg.command)
    {
    case CT_GetUserInfoRequest:
    {
        GetUserInfoRequest request;
        get_request(request, msg.data);
    }
        break;
    default:
        break;
    }


    return 0;
}
