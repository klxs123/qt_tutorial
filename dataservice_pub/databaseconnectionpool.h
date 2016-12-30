#ifndef DATABASECONNECTIONPOOL_H
#define DATABASECONNECTIONPOOL_H

#include "singleton.hpp"

#include <mysql.h>
#include "databaseinfo.hpp"

class DatabaseConnectionPool : public Singleton<DatabaseConnectionPool>
{    
public:
    DatabaseConnectionPool();
    MYSQL* getDatabaseObject();
    int  init(const DatabaseInfo&);
    void clear();

protected:
    MYSQL* m_databaseObject;
};

#endif // DATABASECONNECTIONPOOL_H
