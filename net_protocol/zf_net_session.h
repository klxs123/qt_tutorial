#ifndef __zf_net_session_h__
#define __zf_net_session_h__

#include "net_session.h"

class zf_tcp_session : public tcp_session
{
public:
    zf_tcp_session(int sock);
protected:
    int  handle_recv_data();
};


#endif
