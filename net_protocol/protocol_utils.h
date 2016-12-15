#ifndef __protocol_utils_h__
#define __protocol_utils_h__

#include "protocol_def.h"

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


#endif
