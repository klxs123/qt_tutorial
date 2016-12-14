#ifndef __protocol_utils_h__
#define __protocol_utils_h__

#include "protocol_def.h"

int make_request(const GetUserInfoRequest& request, string& data);

int get_request(GetUserInfoRequest& request, const string& data);

int make_response(const GetUserInfoResponse& response, string& data);

int get_response(GetUserInfoResponse& response, const string& data);


#endif
