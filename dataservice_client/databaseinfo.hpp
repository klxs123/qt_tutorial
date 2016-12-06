#ifndef DATABASEINFO_HPP
#define DATABASEINFO_HPP

#include <string>
#include <stdint.h>

using std::string;

struct DatabaseInfo
{
    string server;
    uint16_t port;
    string user;
    string pwd;
    string database;
    DatabaseInfo():server("192.168.56.1"),port(3306)
      ,user("root"),pwd("root"),database("testdb")
    {
    }
};

#endif // DATABASEINFO_HPP
