#include "protocol_parser.h"

#include <cstring>
#include <malloc.h>


const char* PROTOCOL_START = "!start!";
const char* PROTOCOL_TAIL = "!end!";

#define PKG_HEADER_LEN strlen(PROTOCOL_START)

#define PKG_TAIL_LEN strlen(PROTOCOL_TAIL)

int msg_to_package(const Message &msg, std::string &data)
{
    uint32_t data_len  = strlen(PROTOCOL_START) + strlen(PROTOCOL_TAIL) + sizeof(uint32_t)*2;
    data.reserve(data_len+data.length());
    data+= PROTOCOL_START;
    //此处本应该为数据包编号
    int package_no = 0;
    data.insert(data.length(), (const char*)&package_no, sizeof(uint32_t));

    data.insert(data.length(), (const char*)&msg.command, sizeof(uint32_t));

    data+=msg.data;
    data+=PROTOCOL_TAIL;

    return 0;

}

int package_to_msg(Message &msg, const std::string &pkg)
{
   return package_to_msg(msg, pkg, 0);
}



int package_to_msg(Message &msg, const std::string &pkg, size_t pos)
{
    msg.num = *(uint32_t*)(pkg.data()+pos);

    msg.command = (CommandType)*(uint32_t*)(pkg.data()+pos+sizeof(uint32_t));

    size_t start_index = pos + sizeof(uint32_t)*2;



    msg.data.reserve(pkg.length() - start_index);
    msg.data.insert(0,pkg.data()+ start_index, pkg.length()- start_index);

    return 0;
}
int extract_packages(std::string &data, string& package)
{
    if(data.length() == 0)
    {
        return -1;
    }
    size_t index = data.find(PROTOCOL_START, 0);

    size_t end_index = data.find(PROTOCOL_TAIL, index);

    if(index == string::npos || end_index == string::npos)
    {
        return -1;
    }

    package = data.substr(index+PKG_HEADER_LEN, end_index - index - PKG_HEADER_LEN);

    data.erase(0, end_index + PKG_TAIL_LEN);

    return index;
}

