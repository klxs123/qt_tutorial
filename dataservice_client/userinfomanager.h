#ifndef USERINFOFETCH_H
#define USERINFOFETCH_H

#include <string>
#include <list>
using std::string;
struct UserInfo
{
    string name;
    int salary;
    string pic;
};

class UserInfoManager
{
public:
    UserInfoManager();
    bool fetchUserInfo(const string& name);
    int fetchUserInfo(std::list<UserInfo*>& users);
    int fetchAll(std::list<UserInfo*>& users);
    UserInfo* fullFetchUserInfo(const string &name);
    bool updateUserInfo(const UserInfo* user);
};

#endif // USERINFOFETCH_H
