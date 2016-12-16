#ifndef __protocol_parser_h__
#define __protocol_parser_h__

#include "protocol_def.h"

#include <stdint.h>
#include <string>

using std::string;

//消息对象到要发送的网络包
int msg_to_package(const Message& msg, string& data);
//网络数据包到消息对象
int package_to_msg(Message& msg, const string& pkg);

int package_to_msg(Message &msg, const std::string &pkg, size_t start_pos);
//从数据缓冲区中提取一帧网络数据包
int extract_packages(string& data,string& pkg);
//从数据缓冲区中提取消息对象
int extract_msg(string& data, Message& msg);



//command <---convert----> package
int command_to_package(const GetUserInfoResponse& response, string& data);

#endif
