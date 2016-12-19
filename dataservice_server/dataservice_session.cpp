#include <unistd.h>
#include "dataservice_session.h"
#include <protocol_parser.h>
#include <protocol_utils.h>
#include "data_source.h"
dataservice_session::dataservice_session(int sock):tcp_session(sock)
{

}

int  dataservice_session::handle_recv_data()
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



    Message msg;
    extract_msg(m_data, msg);

    switch(msg.command)
    {
    case CT_GetUserInfoRequest:
    {
        GetUserInfoRequest request;
        get_request(request, msg.data);
        GetUserInfoResponse response;
        process_request(request, response);

        //reuse msg object
        msg.clear();
        msg.command = CT_GetUserInfoResponse;
        //response object to bytes
        make_response(response, msg.data);
        //msg object to package(bytes)
        string data;
        msg_to_package(msg,data);
        //send bytes to client
        ::write(m_sockfd, data.data(), data.length());

    }
        break;
    default:
        break;
    }


    return 0;
}
