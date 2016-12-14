#ifndef __protocol_parser_h__
#define __protocol_parser_h__

#include "protocol_def.h"

#include <stdint.h>
#include <string>

using std::string;




int msg_to_package(const Message& msg, string& data);

int package_to_msg(Message& msg, const string& pkg);

int package_to_msg(Message &msg, const std::string &pkg, size_t pos);

int extract_packages(string& data,string& pkg);



#endif
