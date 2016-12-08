#include "protocol_utils.h"

static const char* field_delimiter = "|";

int make_getuserinfo_request(const GetUserInfoRequest &request, string &data)
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

int get_getuserinfo_request(GetUserInfoRequest &request, const std::string &data)
{

    std::size_t end = data.find(field_delimiter, 0);
    string users_str = data.substr(0,end);
    string fields_str = data.substr(end+1, data.length());
    const char* delimiter = ",";

    split_string(users_str, delimiter, request.users);

    split_string(fields_str, delimiter, request.fields);

    return 0;
}

int make_getuserinfo_response(const GetUserInfoResponse &response, string &data)
{
    uint32_t section_num = response.success?response.users.size(): 0;

    data.append(string((const char*)&section_num, sizeof(uint32_t)));
    if(0 == section_num)
    {
        return 0;
    }

    for(list<  std::pair<string, map<string, string> > >::const_iterator it = response.users.begin(); it != response.users.end(); it++)
    {
        std::pair<string, map<string, string> > user = *it;

    }

    return 0;
}

int get_getuserinfo_response(GetUserInfoResponse &response, const std::string &data)
{
    return 0;
}
