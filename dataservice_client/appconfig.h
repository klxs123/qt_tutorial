#ifndef APPCONFIG_H
#define APPCONFIG_H

#include "databaseinfo.hpp"

class AppConfig
{
public:
    static AppConfig& Instance();
    int init();
    DatabaseInfo& getDatabaseInfo();
    void setDatabaseInfo(const DatabaseInfo& info);
private:
    AppConfig(){};
    AppConfig(const AppConfig&);
    AppConfig& operator=(const AppConfig&);
    DatabaseInfo m_databaseInfo;
};

#endif // APPCONFIG_H
