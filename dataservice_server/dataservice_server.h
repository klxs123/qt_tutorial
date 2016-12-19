#ifndef __zf_server_h__
#define __zf_server_h__


#include <net_server.h>


class dataservice_server : public tcp_server
{
public:
    dataservice_server(const std::string& ip, uint32_t port );
protected:
    int  on_data_arriving(client_sock&);
};



#endif
