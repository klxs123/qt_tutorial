#include "userinfomanager.h"

#include <my_global.h>
#include <mysql.h>

int  finish_with_error(MYSQL *con)
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

bool UserInfoManager::fetchUserInfo(const std::string &name)
{
    return false;
}

int UserInfoManager::fetchUserInfo(std::list<UserInfo *>& users)
{
    return 0;
}

int UserInfoManager::fetchAll(std::list<UserInfo *>& users)
{
    int count = 0;
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return -1;
    }

    if (mysql_real_connect(con, "localhost", "root", "root",
                           "testdb", 3306, NULL, 0) == NULL)
    {
        return finish_with_error(con);
    }

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
    mysql_close(con);
    return count;
}

UserInfo* UserInfoManager::fullFetchUserInfo(const std::string &name)
{
    int count = 0;
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return 0;
    }

    if (mysql_real_connect(con, "localhost", "root", "root",
                           "testdb", 3306, NULL, 0) == NULL)
    {
        finish_with_error(con);
        return 0;
    }

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
    unsigned long *lengths = mysql_fetch_lengths(result);


    UserInfo* puser = new UserInfo;
    puser->name = row[UserTableName];
    puser->salary = atoi(row[UserTableSalary]);

    puser->pic.append(row[UserTableImg], lengths[UserTableImg]);

    mysql_free_result(result);
    mysql_close(con);
    return puser;
}

bool UserInfoManager::updateUserInfo(const UserInfo *user)
{
    if(user == 0)
    {
        return false;
    }
    const char* data = user->pic.c_str();
    int size = user->pic.length();

    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return false;
    }

    if (mysql_real_connect(con, "localhost", "root", "root",
                           "testdb", 3306, NULL, 0) == NULL)
    {
        finish_with_error(con);
        return false;
    }

    /*  为字符串中某些危险字符增加脱字符（反斜线\) (blob?) 防止用户进行SQL注入攻击  */

    char* chunk = (char*)malloc(2*size +1);
    mysql_real_escape_string(con, chunk, data, size);
    const char *st = "update users set pic ='%s' where name = '%s'";
    size_t st_len = strlen(st);
    char *query = (char*)malloc(st_len + 2 * size + 1);
    int len = snprintf(query, st_len + 2 * size + 1, st, chunk, user->name.c_str());
    mysql_real_query(con, query, len);

    mysql_close(con);
    free(chunk);

    return true;
}
