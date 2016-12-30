#include "protocol_utils.h"
#include <cstring>

#ifdef PROTOCOL_DEBUG
#include <stdio.h>
#endif

static const char* field_delimiter = "|";
using namespace std;


int split_string(const string&src, const char* delimiter, std::list<string> &results)
{
    int count = 0;
    std::size_t start = 0;
    while(start <src.length())
    {
        std::size_t end = src.find(delimiter, start);
        results.push_back(src.substr(start, end-start));
        count++;
        start = end+1;
    }


    return count;
}



#define LEN_TYPE uint32_t
const uint32_t LEN_FIELD_LEN = sizeof(LEN_TYPE);

//!注意,"数据段"的长度,由包含该"数据段"的上一级的相关函数插入
/**
1. 字段数据表示->"fieldname":value
   数据表示前面4个字节为该字段的数据长度(不包括表示长度的4字节)
2. 节可以包含多个字段
   开头4字节为该节的数据长度
   后面4字节为该节包含的字段数量
   后面跟(1.)中描述的字段
3. 信息可以包含多个节
   开头4字节为该信息的数据长度
   后面4字节为该信息包含的节的数量
   后面跟(2.)中描述的节
*/
//不包含前面表示4个字节的数据长度,只包含数据:"field_name":value
static int data_to_pair(pair<string, string>& obj, const string& data)
{
    size_t field_name_end_pos = data.find("\":");

    obj.first.insert(0, data.substr(1, field_name_end_pos-1));
    obj.second.insert(0, data.substr(field_name_end_pos+2, data.length() - field_name_end_pos -2));

    return 0;
}

static int pair_to_data(const pair<string, string>& obj, string &data)
{
    uint32_t original_len = data.length();
    data += "\"";
    data += obj.first;
    data += "\":";
    data += obj.second;

    return 0;
}

//pair<string,string> type

static int pair_to_field(const pair<string, string>& obj, string& data)
{
    uint32_t original_len = data.length();
    pair_to_data(obj, data);
    //开头插入本段数据长度(4个字节)
    uint32_t field_data_len = data.length() - original_len;
    data.insert(original_len, (const char*)&field_data_len, LEN_FIELD_LEN);
}

static int field_to_pair(pair<string, string>& obj, const string&data)
{
    LEN_TYPE field_data_len = *(LEN_TYPE*)data.data();

    return  data_to_pair(obj, data.substr(LEN_FIELD_LEN, field_data_len));
}

//boolean type

static int to_field(bool success, string&data)
{
    uint32_t field_data_len = 4;

    data.append((const char*)&field_data_len, LEN_FIELD_LEN);

    uint32_t result = success ? 1:0;
    data.append((const char*)&result, LEN_FIELD_LEN);

    return 0;
}

static int from_field(bool& success, const string &data)
{
    success = *(LEN_TYPE*)(data.data()+LEN_FIELD_LEN) == 0 ? false: true;

    return 0;
}

//不包含表示数据长度的开头4字节
static int data_to_map(map<string, string>& obj, const string& data)
{
    //四个字节表示字段数量
    uint32_t field_num = *(uint32_t*)data.substr(0, LEN_FIELD_LEN).data();
    size_t field_pos = LEN_FIELD_LEN;
    for (int i = 0; i < field_num; i++)
    {
        //每个字段的开头四个字节表示本字段的数据长度(字段长度=数据长度+LEN_FIELD_LEN)
        const string& field_data = data.substr(field_pos + LEN_FIELD_LEN,
            *(uint32_t*)(data.data()  + field_pos));
        pair<string, string> p;
        data_to_pair(p, field_data);
        obj.insert(p);
        field_pos += LEN_FIELD_LEN;
        field_pos += field_data.length();
    }
    return 0;
}
//转换的时候,不会在数据前面插入数据长度
static int map_to_data(const map<string, string>& obj, string& data)
{
    for (map<string, string>::const_iterator mit = obj.begin(); mit != obj.end(); mit++)
    {
       pair_to_field(*mit, data);
    }
    //开头插入包含的数据段的数量
    uint32_t section_num = obj.size();
    data.insert(0, (const char*)&section_num, LEN_FIELD_LEN);
    return 0;
}

static int list_map_to_data(const list< map<string, string> > & obj, string& data)
{
    uint32_t section_num = obj.size();


    if (0 == section_num)
    {
        data.insert(data.length(), string((const char*)&section_num, sizeof(uint32_t)));
        return 0;
    }

    for (list< map<string, string> >::const_iterator it = obj.begin(); it != obj.end(); it++)
    {
        string section_data;
        map_to_data((*it), section_data);
        //再插入该数据段的数据长度
        uint32_t section_data_len = section_data.length();
        section_data.insert(0, (const char*)&section_data_len, LEN_FIELD_LEN);

        data += section_data;
    }
    //开头插入包含的子数据段数量
    data.insert(0, (const char*)&section_num, LEN_FIELD_LEN);
    //开头插入数据段总长度
    uint32_t total_data_len = data.length();
    data.insert(0, (const char*)&total_data_len, LEN_FIELD_LEN);

    return 0;
}

static int data_to_list_map(list<map<string, string> > & obj, const string& data)
{
    //开头四个字节表示本段数据长度
    uint32_t data_len = *(uint32_t*)data.substr(0, LEN_FIELD_LEN).data();
    //后面四个字节表示字段数量
    uint32_t section_num = *(uint32_t*)data.substr(LEN_FIELD_LEN, LEN_FIELD_LEN).data();

    size_t field_pos = LEN_FIELD_LEN + LEN_FIELD_LEN;
    for (int i = 0; i < section_num; i++)
    {
        //每个字段的开头四个字节表示本字段的数据长度(字段长度=数据长度+LEN_FIELD_LEN)
        const string& section_data = data.substr(field_pos+ LEN_FIELD_LEN,
            *(uint32_t*)(data.data() + field_pos) );
        map<string, string>  p;

        data_to_map(p, section_data);

        obj.push_back(p);

        field_pos += LEN_FIELD_LEN;
        field_pos += section_data.length();
    }
    return 0;
}




int make_request(const GetUserInfoRequest &request, string &data)
{
    data.clear();

    for(std::list<string>::const_iterator it = request.users.begin(); it != request.users.end(); it++)
    {
        data.append(*it);
        data.append(",");
    }

    data.append(field_delimiter);

    for(std::list<string>::const_iterator it = request.fields.begin(); it != request.fields.end(); it++)
    {
        data.append(*it);
        data.append(",");
    }

    return 0;
}


int get_request(GetUserInfoRequest &request, const std::string &data)
{

    std::size_t end = data.find(field_delimiter, 0);
    string users_str = data.substr(0,end);
    string fields_str = data.substr(end+1, data.length());
    const char* delimiter = ",";

    split_string(users_str, delimiter, request.users);

    split_string(fields_str, delimiter, request.fields);
#ifdef PROTOCOL_DEBUG
    printf("get request:%s\n", users_str.c_str());
#endif

    return 0;
}

int make_response(const GetUserInfoResponse &response, string &data)
{
    uint32_t section_num = response.success?response.users.size(): 0;

    data.append(string((const char*)&section_num, sizeof(uint32_t)));
    if(0 == section_num)
    {
        return 0;
    }

    string con;
    list_map_to_data(response.users, con);
    data+=con;

    return 0;
}

int get_response(GetUserInfoResponse &response, const std::string &data)
{

    uint32_t section_num = *(uint32_t*)data.data();
    if(0 == section_num)
    {
        response.success = false;
    }
    response.success = true;


    data_to_list_map(response.users, data.substr(sizeof(uint32_t), data.length() - sizeof(uint32_t)));

    return 0;
}

int make_request(const LoginRequest &request, string &data)
{
    return pair_to_field(request.user, data);;
}

int get_request(LoginRequest &request, const string &data)
{
    return field_to_pair(request.user, data);
}

int make_response(const LoginResponse &response, string &data)
{
    return to_field(response.success, data);
}

int get_response(LoginResponse &response, const string &data)
{
    return from_field(response.success, data);
}

int make_request(const UpdateUserInfoRequest &request, string &msg_data)
{
    return 0;
}
