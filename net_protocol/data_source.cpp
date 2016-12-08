#include <algorithm>
#include <cstring>
#include <cstdio>
#include "data_source.h"
using namespace std;

int get_sql(const GetUserInfoRequest &request, std::string &sql)
{
    char buf[1024]={0};

    string fields;

    for(list<string>::const_iterator it = request.fields.begin(); it!= request.fields.end();)
    {
        fields+=*it;
        if(++it != request.fields.end())
            fields+=",";
    }



    const char* format = "select %s from users";
    snprintf(buf, 1024-1, format, fields.c_str());

    sql = buf;

    list<string>::const_iterator it = find(request.users.begin(), request.users.end(), "*");

    if(it != request.users.end())
    {
        return 0;
    }

    sql+= " where ";

    for(list<string>::const_iterator it = request.users.begin(); it!= request.users.end();)
    {
        sql += " name='";
        sql += *it;
        sql += "'";
        if(++it != request.users.end())
        {
            sql+=" or ";
        }
    }

    return 0;
}

int process_request(const GetUserInfoRequest &request, GetUserInfoResponse &response)
{
    string sql;
    if(get_sql(request, sql) != 0)
    {
        return -1;
    }


    return 0;
}
