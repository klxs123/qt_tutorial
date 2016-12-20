#ifndef __protocol_utils_h__
#define __protocol_utils_h__

#include "protocol_def.h"



int split_string(const string&src, const char* delimiter, std::list<string> &results);

//每条命令都分为请求与回应，都需要序列化、反序列化，这样，每条命令都需要4个工具函数

//login command
int make_request(const LoginRequest& request, string& msg_data);
int get_request(LoginRequest& request, const string& msg_data);
int make_response(const LoginResponse& response, string& msg_data);
int get_response(LoginResponse& response, const string& msg_data);


//getuserinfo command
int make_request(const GetUserInfoRequest& request, string& msg_data);
int get_request(GetUserInfoRequest& request, const string& msg_data);
int make_response(const GetUserInfoResponse& response, string& msg_data);
int get_response(GetUserInfoResponse& response, const string& msg_data);


//updateuserinfo command
int make_request(const UpdateUserInfoRequest& request, string& msg_data);
int get_request(UpdateUserInfoRequest& request, const string& msg_data);
int make_response(const UpdateUserInfoResponse& response, string& msg_data);
int get_response(UpdateUserInfoResponse& response, const string& msg_data);



#endif
