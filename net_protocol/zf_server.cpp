#include "protocol_parser.h"
#include "protocol_utils.h"
#include "data_source.h"

#include "zf_server.h"
#include "zf_net_session.h"

#include <unistd.h>

#include <cstdio>
zf_server::zf_server(const std::string &ip, uint32_t port):tcp_server(ip, port)
{

}


int  zf_server::on_data_arriving(client_sock& cs)
{
    char buf[1024] = {0};
    ssize_t size = ::read(cs.sockfd, buf, 1024);
    cs.data.append(buf, size);

    string pkg;

    if( extract_packages(cs.data, pkg) == -1)
    {
        return -1;
    }

    Message msg;

    package_to_msg(msg, pkg);

    LoginRequest request;

    if( get_request(request, msg.data) == -1)
    {
        return -1;
    }
#ifdef PROTOCOL_DEBUG
    printf("recv login request:%s:%s\n", request.user.first.c_str(), request.user.second.c_str());
#endif
    //authen request data
    LoginResponse response;
    response.success = true;

    msg.clear();

    make_response(response, msg.data);
    string data;
    msg_to_package(msg, data);

    ::write(cs.sockfd, data.data(), data.length());


    //todo...

    tcp_session* pts = new zf_tcp_session(cs.sockfd);
    pts->open();

    m_sessions.push_back(pts);

    return 0;
}


