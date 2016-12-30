#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <string>
#include <stdint.h>

struct ConnectInfo
{
    std::string ip;
    uint16_t port;
    std::string user;
    std::string pwd;
};

class AppConfig
{
public:
    static AppConfig& Instance();
    int init();
    ConnectInfo &getConnectInfo();

private:
    AppConfig(){}
    AppConfig(const AppConfig&);
    AppConfig& operator=(const AppConfig&);
    ConnectInfo m_connectInfo;

};

#endif // APPCONFIG_H
