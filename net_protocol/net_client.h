#ifndef __net_client_h__
#define __net_client_h__

#include "net_def.h"

class tcp_client
{
public:
    enum error
    {
        NOERROR,
        CONNECTED,
        CLOSED,
    };

    tcp_client();
    virtual ~ tcp_client();
    int connect(const std::string& ip, uint16_t port);
    //将读取的数据追加到参数<data>
    int read(std::string& data, size_t len);
    int write(const std::string& data);

    void close();
protected:
    bool m_connected;
    int  m_sockfd;
    NetPoint m_point;
    uint8_t *m_buffer;
    size_t m_buffer_size;
};


#endif
