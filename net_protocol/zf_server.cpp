#include "protocol_parser.h"
#include "protocol_utils.h"
#include "data_source.h"

#include "zf_server.h"

#include <unistd.h>

#include <cstdio>
zf_server::zf_server(const std::string &ip, uint32_t port):tcp_server(ip, port)
{

}


void zf_server::on_data_arriving(client_sock& cs)
{
    char buf[1024] = {0};
    ssize_t size = ::read(cs.sockfd, buf, 1024);
    cs.data.append(buf, size);

    string pkg;

    if( extract_packages(cs.data, pkg) == -1)
    {
        return;
    }

    Message msg;

    package_to_msg(msg, pkg);

    GetUserInfoRequest request;

    get_request(request, msg.data);

    string sql;
    get_sql(request, sql);

    printf("get request:%s\n", sql.c_str());

}
