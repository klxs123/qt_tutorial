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

DatabaseInfo &AppConfig::getDatabaseInfo()
{
    return m_databaseInfo;
}

void AppConfig::setDatabaseInfo(const DatabaseInfo &info)
{
    m_databaseInfo = info;
}

