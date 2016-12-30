#include "appconfig.h"

AppConfig &AppConfig::Instance()
{
    static AppConfig config;
    return config;
}

int AppConfig::init()
{
    return 0;
}

ConnectInfo &AppConfig::getConnectInfo()
{
    return m_connectInfo;
}


