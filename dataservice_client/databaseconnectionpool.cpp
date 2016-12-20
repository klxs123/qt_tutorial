#include <cassert>
#include "databaseconnectionpool.h"

int  finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    return -1;
}

DatabaseConnectionPool::DatabaseConnectionPool():m_databaseObject(0)
{

}

MYSQL *DatabaseConnectionPool::getDatabaseObject()
{
    assert(m_databaseObject !=0);//防止还没有初始化就调用此函数
    return m_databaseObject;
}

int  DatabaseConnectionPool::init(const DatabaseInfo &info)
{
    if(m_databaseObject != 0)
    {
        mysql_close(m_databaseObject);
    }

    m_databaseObject = mysql_init(NULL);

    if (m_databaseObject == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return -1;
    }

    if (mysql_real_connect(m_databaseObject, info.server.c_str(), info.user.c_str()
                           , info.pwd.c_str(), info.database.c_str(), info.port, NULL, 0) == NULL)
    {
        return finish_with_error(m_databaseObject);
    }

    return 0;
}

void DatabaseConnectionPool::clear()
{
    if(m_databaseObject ==0)
    {
        return;
    }

    mysql_close(m_databaseObject);
    m_databaseObject = 0;
}
