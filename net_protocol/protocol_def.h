#ifndef __protocol_def_h__
#define __protocol_def_h__

#include <stdint.h>
#include <string>
#include <list>
#include <map>
#include <vector>
using std::string;
using std::list;
using std::map;

enum CommandType
{
    CT_LoginRequest,
    CT_LoginResponse,
    CT_GetUserInfoRequest,//['username','fields',.....]
    //   | field num |field01|field02|.......|field0n|
    //   |--4byte----| field format:
    //               |field len(4bytes)|field data('fieldname':value)|

    CT_GetUserInfoResponse,
    CT_UpdateUserInfoRequest,
    CT_UpdateUserInfoResponse,

    CT_AddUser,//['field01'='xxxx', 'field02'='xxxx',....]
    CT_SetUserInfo,//['username','field01'='xxxx', 'field02'='xxxx',....]
    CT_DelUser,//['username']
};


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
