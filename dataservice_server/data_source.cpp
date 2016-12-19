#include <algorithm>
#include <cstring>
#include <cstdio>
#include "data_source.h"

#include <mysql.h>

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

static void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

int process_request(const GetUserInfoRequest &request, GetUserInfoResponse &response)
{
    string sql;
    if(get_sql(request, sql) != 0)
    {
        return -1;
    }

    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(con, "192.168.56.1", "root", "root",
                           "testdb", 0, NULL, 0) == NULL)
    {
        finish_with_error(con);
    }

    if (mysql_query(con, sql.c_str()))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL)
    {
        finish_with_error(con);
    }

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        map<string,string> user_info;
        int index = 0;
        for(list<string>::const_iterator it = request.fields.begin(); it!=request.fields.end(); it++, index++)
        {
            user_info.insert(pair<string,string>(*it, row[index] ? row[index] : "NULL"));
#ifdef DATASOURCE_DEBUG
            printf("%s ", row[index] ? row[index] : "NULL");
#endif

        }
        response.users.push_back(user_info);
#ifdef DATASOURCE_DEBUG
        printf("\n");
#endif
    }

    mysql_free_result(result);
    mysql_close(con);

    response.success = response.users.size() !=0 ? true: false;

    return 0;
}
