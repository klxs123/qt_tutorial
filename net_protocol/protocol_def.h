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
    CT_GetUserInfoRequest,//['username','fields',.....]
    //   | field num |field01|field02|.......|field0n|
    //   |--4byte----| field format:
    //               |field len(4bytes)|field data('fieldname':value)|

    CT_GetUserInfoResponse,
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
