#include "userinfomanager.h"

#include <my_global.h>
#include <mysql.h>
#include "databaseconnectionpool.h"

static int  finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    return -1;
}

enum UserTableColumns
{
    UserTableName =0,
    UserTableSalary,
    UserTableImg
};

UserInfoManager::UserInfoManager()
{

}


int UserInfoManager::fetchAll(std::list<UserInfo *>& users)
{
    int count = 0;
    MYSQL *con = DatabaseConnectionPool::Instance().getDatabaseObject();

    if (mysql_query(con, "SELECT name FROM users"))
    {
        return finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        return finish_with_error(con);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        UserInfo* puser = new UserInfo;
        puser->name = row[0];
        users.push_back(puser);
        count++;
    }

    mysql_free_result(result);

    return count;
}

UserInfo* UserInfoManager::fullFetchUserInfo(const std::string &name)
{
    MYSQL *con = DatabaseConnectionPool::Instance().getDatabaseObject();

    char sql[1024] = {0};
    snprintf(sql, 1023,  "SELECT * FROM users where name='%s';", name.c_str());
    if (mysql_query(con, sql))
    {
        finish_with_error(con);
        return 0;
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
        return 0;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if(row == 0)
    {
        mysql_free_result(result);
        return 0;
    }
    unsigned long *lengths = mysql_fetch_lengths(result);


    UserInfo* puser = new UserInfo;
    puser->name = row[UserTableName];
    puser->salary = atoi(row[UserTableSalary]);

    puser->pic.append(row[UserTableImg], lengths[UserTableImg]);

    mysql_free_result(result);

    return puser;
}

bool UserInfoManager::updateUserInfo(const UserInfoEx *user)
{
    if(user == 0)
    {
        return false;
    }
    const char* data = user->pic.second.c_str();
    size_t size = user->pic.second.length();

    MYSQL *con = DatabaseConnectionPool::Instance().getDatabaseObject();

    /*  为字符串中某些危险字符增加脱字符（反斜线\) (blob?) 防止用户进行SQL注入攻击  */

    char* chunk = (char*)malloc(2*size +1);
    mysql_real_escape_string(con, chunk, data, size);
    const char *st = "update users set pic ='%s' where name = '%s'";
    size_t st_len = strlen(st);
    char *query = (char*)malloc(st_len + 2 * size + 1);
    int len = snprintf(query, st_len + 2 * size + 1, st, chunk, user->name.second.c_str());
    mysql_real_query(con, query, len);

    free(chunk);

    return true;
}

int UserInfoManager::addUser(const UserInfo& user)
{    
    const char* sql_format = "insert into users values('%s', %d, '%s');";

    const char* data = user.pic.c_str();
    int pic_size = user.pic.length();

    MYSQL *con = DatabaseConnectionPool::Instance().getDatabaseObject();

    /*  为字符串中某些危险字符增加脱字符（反斜线\) (blob?) 防止用户进行SQL注入攻击  */

    char* chunk = (char*)malloc(2*pic_size +1);
    mysql_real_escape_string(con, chunk, data, pic_size);

    size_t sql_len =  2*pic_size  +1024;

    char* sql = (char*)malloc(sql_len);

    sql_len =  snprintf(sql, sql_len, sql_format, user.name.c_str(), user.salary, chunk );

    mysql_real_query(con, sql,sql_len);


    free(sql);
    free(chunk);


    return 0;
}

int UserInfoManager::delUser(const std::string &name)
{
     MYSQL *con = DatabaseConnectionPool::Instance().getDatabaseObject();

     const char* sql_format = "delete from users where name='%s'";
     char sql[1024] = {0};
     int sql_len = snprintf(sql, 1024, sql_format, name.c_str());
     return mysql_real_query(con, sql, sql_len);
}
