#ifndef __protocol_utils_h__
#define __protocol_utils_h__

#include "protocol_def.h"

int make_getuserinfo_request(const GetUserInfoRequest& request, string& data);

int get_getuserinfo_request(GetUserInfoRequest& request, const string& data);

int make_getuserinfo_response(const GetUserInfoResponse& response, string& data);

int get_getuserinfo_response(GetUserInfoResponse& response, const string& data);


#endif
