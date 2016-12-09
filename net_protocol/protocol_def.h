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
enum MessageType
{
    RequestMessage,
    ResponseMessage,
};

enum CommandType
{
    CT_GetUserInfo,//['username','fields',.....]
    //   | field num |field01|field02|.......|field0n|
    //   |--4byte----| field format:
    //               |field len(4bytes)|field data('fieldname':value)|
    CT_AddUser,//['field01'='xxxx', 'field02'='xxxx',....]
    CT_SetUserInfo,//['username','field01'='xxxx', 'field02'='xxxx',....]
    CT_DelUser,//['username']
};

struct DataBuffer
{
    uint8_t* data;
    uint32_t len;
};

struct Message
{
    MessageType type;
    uint32_t  num;//serial number
    CommandType command;
    DataBuffer data;
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
