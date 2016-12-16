#ifndef __data_source_h__
#define __data_source_h__


#include "protocol_def.h"



#include <string>

//just for test
int get_sql(const GetUserInfoRequest& request, std::string& sql);


int process_request(const GetUserInfoRequest& request, GetUserInfoResponse& response);


#endif
