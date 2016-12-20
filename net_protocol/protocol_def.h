#ifndef __protocol_def_h__
#define __protocol_def_h__

#include <stdint.h>
#include <string>
#include <list>
#include <map>
#include <vector>

#include "command_def.h"
using std::string;
using std::list;
using std::map;



struct Message
{
    uint32_t  num;//serial number
    CommandType command;
    string data;
    size_t length() const
    {
        return data.length()+ sizeof(uint32_t)*2;
    }

    void clear()
    {
        data.clear();
        num = 0;
    }
};

struct LoginRequest
{
    std::pair<string,string> user;
};

struct LoginResponse
{
    bool success;
};

struct GetUserInfoRequest
{
    std::list<string> users;
    std::list<string> fields;
};

struct GetUserInfoResponse
{
    bool success;

    list< map<string, string>  > users;
};

//规定更新数据的请求必须包含用户ID(name?)而且要安排在第一个
struct UpdateUserInfoRequest
{
    list< map<string, string>  > datas;
};

struct UpdateUserInfoResponse
{
    bool success;

};


struct AddUserRequest
{
    map<string,string> datas;
};
struct AddUserResponse
{
    bool success;
    string msg;
};

#endif
