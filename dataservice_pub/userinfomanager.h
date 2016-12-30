#ifndef USERINFOFETCH_H
#define USERINFOFETCH_H

#include <string>
#include <list>
#include <utility>
using std::string;
using std::pair;
struct UserInfo
{
    string name;
    int salary;
    string pic;
};

struct UserInfoEx
{
    pair<bool, string> name;
    pair<bool, int> salary;
    pair<bool, string> pic;
};

#include "singleton.hpp"

class UserInfoManager : public Singleton<UserInfoManager>
{
public:
    UserInfoManager();
    //bool fetchUserInfo(const string& name);
   // int fetchUserInfo(std::list<UserInfo*>& users);
    int fetchAll(std::list<UserInfo*>& users);
    UserInfo* fullFetchUserInfo(const string &name);
    bool updateUserInfo(const UserInfoEx* user);
    int addUser(const UserInfo& user);
    int delUser(const string& name);
};

#endif // USERINFOFETCH_H
