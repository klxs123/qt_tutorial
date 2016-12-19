#ifndef __zf_net_session_h__
#define __zf_net_session_h__

#include <net_session.h>

class dataservice_session : public tcp_session
{
public:
    dataservice_session(int sock);
protected:
    int  handle_recv_data();
};


#endif
