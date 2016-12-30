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
    return -1;
}

UserInfo* UserInfoManager::fullFetchUserInfo(const std::string &name)
{

    return 0;
}

bool UserInfoManager::updateUserInfo(const UserInfoEx *user)
{
    if(user == 0 || (!user->pic.first && !user->salary.first))
    {
        return false;
    }

    const char* data = user->pic.second.c_str();
    size_t size = user->pic.second.length();


    return false;
}

int UserInfoManager::addUser(const UserInfo& user)
{


    return 0;
}

int UserInfoManager::delUser(const std::string &name)
{
    return -1;
}
