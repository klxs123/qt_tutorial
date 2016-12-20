
#ifndef __command_def_h__
#define __command_def_h__

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
    CT_ListCommandReqeust,
    CT_ListCommandResponse,
};

#endif
